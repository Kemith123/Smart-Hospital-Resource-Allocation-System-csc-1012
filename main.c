#include <stdio.h>
#include <stdlib.h>

#define MAX_PATIENTS 100


/* Patient data */
char patientNames[MAX_PATIENTS][50];
int patientAges[MAX_PATIENTS];
int urgencyLevels[MAX_PATIENTS];
int specialtyIDs[MAX_PATIENTS];
int admittedToWard[MAX_PATIENTS];
int wardIDs[MAX_PATIENTS];
int admissionDays[MAX_PATIENTS];

float waitingTimes[MAX_PATIENTS];
float baseFees[MAX_PATIENTS];
float surcharges[MAX_PATIENTS];
float wardCosts[MAX_PATIENTS];
float grossTotals[MAX_PATIENTS];
float discounts[MAX_PATIENTS];
float finalBills[MAX_PATIENTS];
int assignedBedNumbers[MAX_PATIENTS];

int patientCount = 0;
int specialtyQueue[4] = {0};

/* Doctor specialties */
const char specialtyNames[4][30] = { "General Practice","Paediatrics", "Cardiology","Neurology"};

const float consultationFees[4] = {1500.00,2500.00, 4500.00, 5000.00};

const char consultationTimes[4][30] = { "15 mins","20 mins","30 mins","30 mins"};
const int consultationTimeMinutes[4] = {15, 20, 30,30};


const int dailyPatientCaps[4] = { 30, 20,12,10};


/* Hospital wards */
const char wardNames[4][50] = {"General Ward",  "Paediatric Ward", "Surgical Ward","ICU (Intensive Care Unit)"};

const float wardDailyRates[4] = { 3000.00,6000.00,12000.00, 25000.00};

const int wardCapacities[4] = { 20,10,10,5};


/* Bed occupancy */
int bedOccupancy[4][20] = {0};


/* Function prototypes */
void displayMenu();
void registerPatient();
void allocateBed(int patientIndex);
void displayBedOccupancy();
float calculateWaitingTime(int specialtyID);

float calculateSurcharge(int patientIndex);
float calculateWardCost(int patientIndex);
float calculateGrossTotal(int patientIndex);
float calculateDiscount(int patientIndex);
float calculateFinalBill(int patientIndex);
void displayPatientBill(int patientIndex);
void displayPatientPriority();
void generateReports();
void saveBedStatus();
void loadBedStatus();
void savePatientRecord(int patientIndex);

int main()
{
    int choice;
    loadBedStatus();

    do
    {
        displayMenu();

        printf("Enter your choice: ");
        scanf("%d", &choice);

        switch(choice)
        {
            case 1:
                registerPatient();
                break;

            case 2:
                displayBedOccupancy();
                break;

            case 3:
                 displayPatientPriority();
                break;

            case 4:
                generateReports();
                break;

            case 5:
                printf("\nExiting program...\n");
                break;

            default:
                printf("\nInvalid choice. Please try again.\n");
        }

    } while(choice != 5);


 return 0;

}

void displayMenu()
{
    printf("\n========================================\n");
    printf("       SMART HOSPITAL SYSTEM\n");
    printf("========================================\n");
    printf("1. Register Patient\n");
    printf("2. Display Bed Occupancy\n");
    printf("3. Display Patient Priority\n");
    printf("4. Generate Reports\n");
    printf("5. Exit\n");
    printf("========================================\n");
}

void registerPatient()
{
    printf("\n========================================\n");
    printf("          PATIENT REGISTRATION\n");
    printf("========================================\n");

    printf("Enter patient name: ");
    scanf(" %[^\n]", patientNames[patientCount]);

    printf("Enter patient age: ");
    scanf("%d", &patientAges[patientCount]);

    printf("\nEmergency / Triage Level\n");
    printf("1. Normal\n");
    printf("2. Urgent\n");
    printf("3. Critical\n");
    
    printf("Enter urgency level: ");
    scanf("%d", &urgencyLevels[patientCount]);
     if(urgencyLevels[patientCount] < 1 ||urgencyLevels[patientCount] > 3)
     {
      printf("Invalid urgency level. Enter 1, 2, or 3: ");
      scanf("%d", &urgencyLevels[patientCount]);
     }

    printf("\nSpecialty\n");
    printf("1. General Practice\n");
    printf("2. Paediatrics\n");
    printf("3. Cardiology\n");
    printf("4. Neurology\n");
    
    printf("Enter specialty ID: ");
    scanf("%d", &specialtyIDs[patientCount]);
     if(specialtyIDs[patientCount] < 1 ||
      specialtyIDs[patientCount] > 4)
    {
    printf("Invalid specialty. Enter a number from 1 to 4: ");
    scanf("%d", &specialtyIDs[patientCount]);
    }

    printf("\nIs admitted to ward?\n");
    printf("1. Yes\n");
    printf("0. No\n");
    
    printf("Enter choice: ");
    scanf("%d", &admittedToWard[patientCount]);
    while(admittedToWard[patientCount] != 0 && admittedToWard[patientCount] != 1)
     {
    printf("Invalid choice. Enter 1 for Yes or 0 for No: ");
    scanf("%d", &admittedToWard[patientCount]);
     }

    if(admittedToWard[patientCount] == 1)
    {
        printf("Enter ward ID (1-4): ");
        scanf("%d", &wardIDs[patientCount]);
        while(wardIDs[patientCount] < 1 || wardIDs[patientCount] > 4)
          {
              printf("Invalid ward. Enter a number from 1 to 4: ");
              scanf("%d", &wardIDs[patientCount]);
          }

        printf("Enter number of days admitted: ");
        scanf("%d", &admissionDays[patientCount]);
        while(admissionDays[patientCount] <= 0)
          {
            printf("Invalid number of days. Enter a positive number: ");
            scanf("%d", &admissionDays[patientCount]);
          }
    }
    else
    {
        wardIDs[patientCount] = 0;
        admissionDays[patientCount] = 0;
    }

baseFees[patientCount] =
consultationFees[specialtyIDs[patientCount] - 1];

waitingTimes[patientCount] =
    calculateWaitingTime(specialtyIDs[patientCount]);

surcharges[patientCount] =
    calculateSurcharge(patientCount);

wardCosts[patientCount] =
    calculateWardCost(patientCount);

grossTotals[patientCount] =
    calculateGrossTotal(patientCount);

discounts[patientCount] =
    calculateDiscount(patientCount);

finalBills[patientCount] =
    calculateFinalBill(patientCount);

patientCount++;

  if(admittedToWard[patientCount - 1] == 1)
   {
    allocateBed(patientCount - 1);
   }
    


printf("\nPatient registered successfully!\n");
displayPatientBill(patientCount - 1);
savePatientRecord(patientCount - 1);
    
}

void allocateBed(int patientIndex)
{
    int wardIndex;
    int bedIndex;
    int bedFound = 0;

    wardIndex = wardIDs[patientIndex] - 1;

    for(bedIndex = 0; bedIndex < wardCapacities[wardIndex]; bedIndex++)
    {
        if(bedOccupancy[wardIndex][bedIndex] == 0)
        {
            bedOccupancy[wardIndex][bedIndex] = 1;

            assignedBedNumbers[patientIndex] = bedIndex + 1;

            saveBedStatus();

            bedFound = 1;

            printf("\nBed allocated successfully!\n");
            printf("Ward : %s\n", wardNames[wardIndex]);
            printf("Bed  : #%d\n", assignedBedNumbers[patientIndex]);

            break;
        }
    }

    if(bedFound == 0)
    {
        assignedBedNumbers[patientIndex] = 0;

        printf("\nNo available beds in %s.\n",
               wardNames[wardIndex]);

        printf("Patient cannot be allocated a bed.\n");
    }
}


void displayBedOccupancy()
{
    int wardIndex;
    int bedIndex;

    printf("\n========================================\n");
    printf("          BED OCCUPANCY\n");
    printf("========================================\n");

    for(wardIndex = 0; wardIndex < 4; wardIndex++)
    {
        printf("\nWard: %s\n", wardNames[wardIndex]);

        for(bedIndex = 0;
            bedIndex < wardCapacities[wardIndex];
            bedIndex++)
        {
            printf("Bed %02d : ",
                   bedIndex + 1);

            if(bedOccupancy[wardIndex][bedIndex] == 0)
            {
                printf("Available\n");
            }
            else
            {
                printf("Occupied\n");
            }
        }
    }
}


float calculateWaitingTime(int specialtyID)
{
    int specialtyIndex;
    float waitingTime;

    specialtyIndex = specialtyID - 1;

    waitingTime = specialtyQueue[specialtyIndex]
                  * consultationTimeMinutes[specialtyIndex];

    specialtyQueue[specialtyIndex]++;

    return waitingTime;
}

float calculateSurcharge(int patientIndex)
{
    float surcharge = 0;

    if(urgencyLevels[patientIndex] == 1)
    {
        surcharge = 0;
    }
    else if(urgencyLevels[patientIndex] == 2)
    {
        surcharge = baseFees[patientIndex] * 0.20;
    }
    else if(urgencyLevels[patientIndex] == 3)
    {
        surcharge = baseFees[patientIndex] * 0.50;
    }

    return surcharge;
}

float calculateWardCost(int patientIndex)
{
    float wardCost = 0;

    if(admittedToWard[patientIndex] == 1)
    {
        int wardIndex = wardIDs[patientIndex] - 1;

        wardCost = admissionDays[patientIndex]
                   * wardDailyRates[wardIndex];
    }

    return wardCost;
}

float calculateGrossTotal(int patientIndex)
{
    float grossTotal;

    grossTotal = baseFees[patientIndex]
                 + surcharges[patientIndex]
                 + wardCosts[patientIndex];

    return grossTotal;
}

float calculateDiscount(int patientIndex)
{
    float discount = 0;

    if(patientAges[patientIndex] < 5 ||
       patientAges[patientIndex] > 65)
    {
        discount = grossTotals[patientIndex] * 0.15;
    }

    return discount;
}

float calculateFinalBill(int patientIndex)
{
    float finalBill;

    finalBill = grossTotals[patientIndex]
                - discounts[patientIndex];

    return finalBill;
}

void displayPatientBill(int patientIndex)
{
    int specialtyIndex;
    int wardIndex;

    specialtyIndex = specialtyIDs[patientIndex] - 1;

    printf("\n====================================================\n");
    printf("           SMART HOSPITAL ADMISSION & BILL\n");
    printf("----------------------------------------------------\n");

    printf("Patient ID : PAT-%04d\n", 1001 + patientIndex);
    printf("Patient Name : %s\n", patientNames[patientIndex]);

    if(patientAges[patientIndex] < 5 ||
       patientAges[patientIndex] > 65)
    {
        printf("Age : %d Years (15%% Subsidy Eligible)\n",
               patientAges[patientIndex]);
    }
    else
    {
        printf("Age : %d Years\n",
               patientAges[patientIndex]);
    }

    printf("Specialty : %s\n",
           specialtyNames[specialtyIndex]);

    if(admittedToWard[patientIndex] == 1)
    {
        wardIndex = wardIDs[patientIndex] - 1;

        printf("Assigned Ward : %s (Bed #%02d)\n",
               wardNames[wardIndex],
               assignedBedNumbers[patientIndex]);
    }
    else
    {
        printf("Assigned Ward : Outpatient / OPD\n");
    }

    printf("Urgency Level : ");

    if(urgencyLevels[patientIndex] == 1)
    {
        printf("Level 1 (Normal)\n");
    }
    else if(urgencyLevels[patientIndex] == 2)
    {
        printf("Level 2 (Urgent)\n");
    }
    else
    {
        printf("Level 3 (Critical)\n");
    }

    printf("----------------------------------------------------\n");

    printf("Base Consultation Fee : LKR %.2f\n",
           baseFees[patientIndex]);

    if(urgencyLevels[patientIndex] == 1)
    {
        printf("Emergency Surcharge : LKR %.2f (0%%)\n",
               surcharges[patientIndex]);
    }
    else if(urgencyLevels[patientIndex] == 2)
    {
        printf("Emergency Surcharge : LKR %.2f (20%%)\n",
               surcharges[patientIndex]);
    }
    else
    {
        printf("Emergency Surcharge : LKR %.2f (50%%)\n",
               surcharges[patientIndex]);
    }

    if(admittedToWard[patientIndex] == 1)
    {
        printf("Ward Stay Cost (%d Days) : LKR %.2f\n",
               admissionDays[patientIndex],
               wardCosts[patientIndex]);
    }
    else
    {
        printf("Ward Stay Cost : LKR 0.00\n");
    }

    printf("----------------------------------------------------\n");

    printf("Gross Total Bill : LKR %.2f\n",
           grossTotals[patientIndex]);

    printf("Age Subsidy Discount : LKR -%.2f\n",
           discounts[patientIndex]);

    printf("----------------------------------------------------\n");

    printf("Final Payable Amount : LKR %.2f\n",
           finalBills[patientIndex]);

    printf("Estimated Waiting Time : %.2f mins",
           waitingTimes[patientIndex]);

    if(waitingTimes[patientIndex] == 0)
    {
        printf(" (Immediate Attention)");
    }

    printf("\n");
    printf("====================================================\n");
}

void displayPatientPriority()
{
    int priorityOrder[MAX_PATIENTS];
    int i;
    int j;
    int temp;

    for(i = 0; i < patientCount; i++)
    {
        priorityOrder[i] = i;
    }

    /* Stable sorting based on urgency level */
    for(i = 0; i < patientCount - 1; i++)
    {
        for(j = 0; j < patientCount - i - 1; j++)
        {
            if(urgencyLevels[priorityOrder[j]]
               < urgencyLevels[priorityOrder[j + 1]])
            {
                temp = priorityOrder[j];

                priorityOrder[j] =
                    priorityOrder[j + 1];

                priorityOrder[j + 1] = temp;
            }
        }
    }

    printf("\n========================================\n");
    printf("          PATIENT PRIORITY\n");
    printf("========================================\n");

    if(patientCount == 0)
    {
        printf("No patients registered.\n");
        return;
    }

    for(i = 0; i < patientCount; i++)
    {
        int index = priorityOrder[i];

        printf("\nPriority %d\n", i + 1);
        printf("Patient ID : PAT-%04d\n",
               1001 + index);
        printf("Patient Name : %s\n",
               patientNames[index]);

        printf("Urgency Level : ");

        if(urgencyLevels[index] == 3)
        {
            printf("Level 3 (Critical)\n");
        }
        else if(urgencyLevels[index] == 2)
        {
            printf("Level 2 (Urgent)\n");
        }
        else
        {
            printf("Level 1 (Normal)\n");
        }

        printf("Specialty : %s\n",
               specialtyNames[specialtyIDs[index] - 1]);

        printf("Estimated Waiting Time : %.2f mins\n",
               waitingTimes[index]);

        printf("----------------------------------------\n");
    }
}

void generateReports()
{
    int normalCount = 0;
    int urgentCount = 0;
    int criticalCount = 0;

    float totalRevenue = 0;
    float totalDiscounts = 0;

    int highestPatientIndex = -1;
    float highestBill = 0;

    int wardIndex;
    int bedIndex;
    int occupiedBeds;
    float occupancyPercentage;

    int i;

    /* Count patients by urgency */
    for(i = 0; i < patientCount; i++)
    {
        if(urgencyLevels[i] == 1)
        {
            normalCount++;
        }
        else if(urgencyLevels[i] == 2)
        {
            urgentCount++;
        }
        else if(urgencyLevels[i] == 3)
        {
            criticalCount++;
        }

        totalRevenue += finalBills[i];
        totalDiscounts += discounts[i];

        if(highestPatientIndex == -1 ||
           finalBills[i] > highestBill)
        {
            highestBill = finalBills[i];
            highestPatientIndex = i;
        }
    }

    printf("\n===============================================\n");
    printf("           HOSPITAL PERFORMANCE REPORT\n");
    printf("===============================================\n");

    /* Patient summary */
    printf("\nPATIENT SUMMARY\n");
    printf("-----------------------------------------------\n");
    printf("Total Patients Registered : %d\n",
           patientCount);

    printf("Normal Patients (Level 1) : %d\n",
           normalCount);

    printf("Urgent Patients (Level 2) : %d\n",
           urgentCount);

    printf("Critical Patients (Level 3) : %d\n",
           criticalCount);

    /* Financial summary */
    printf("\nFINANCIAL SUMMARY\n");
    printf("-----------------------------------------------\n");

    printf("Total Revenue : LKR %.2f\n",
           totalRevenue);

    printf("Total Discounts Granted : LKR %.2f\n",
           totalDiscounts);

    /* Bed occupancy */
    printf("\nBED OCCUPANCY\n");
    printf("-----------------------------------------------\n");

    for(wardIndex = 0; wardIndex < 4; wardIndex++)
    {
        occupiedBeds = 0;

        for(bedIndex = 0;
            bedIndex < wardCapacities[wardIndex];
            bedIndex++)
        {
            if(bedOccupancy[wardIndex][bedIndex] == 1)
            {
                occupiedBeds++;
            }
        }

        occupancyPercentage =
            ((float)occupiedBeds / wardCapacities[wardIndex])
            * 100;

        printf("%s : %d/%d beds occupied (%.2f%%)\n",
               wardNames[wardIndex],
               occupiedBeds,
               wardCapacities[wardIndex],
               occupancyPercentage);
    }

    /* Highest paying patient */
    printf("\nHIGHEST-PAYING PATIENT\n");
    printf("-----------------------------------------------\n");

    if(highestPatientIndex == -1)
    {
        printf("No patients registered.\n");
    }
    else
    {
        printf("Patient Name : %s\n",
               patientNames[highestPatientIndex]);

        printf("Patient ID : PAT-%04d\n",
               1001 + highestPatientIndex);

        printf("Total Bill : LKR %.2f\n",
               finalBills[highestPatientIndex]);
    }

    printf("===============================================\n");
}

void saveBedStatus()
{
    FILE *file;
    int wardIndex;
    int bedIndex;

    file = fopen("beds_status.txt", "w");

    if(file == NULL)
    {
        printf("\nUnable to save bed status.\n");
        return;
    }

    for(wardIndex = 0; wardIndex < 4; wardIndex++)
    {
        for(bedIndex = 0;
            bedIndex < wardCapacities[wardIndex];
            bedIndex++)
        {
            fprintf(file, "%d ",
                    bedOccupancy[wardIndex][bedIndex]);
        }

        fprintf(file, "\n");
    }

    fclose(file);
}

void loadBedStatus()
{
    FILE *file;
    int wardIndex;
    int bedIndex;

    file = fopen("beds_status.txt", "r");

    if(file == NULL)
    {
        return;
    }

    for(wardIndex = 0; wardIndex < 4; wardIndex++)
    {
        for(bedIndex = 0;
            bedIndex < wardCapacities[wardIndex];
            bedIndex++)
        {
            fscanf(file,
                   "%d",
                   &bedOccupancy[wardIndex][bedIndex]);
        }
    }

    fclose(file);
}

void savePatientRecord(int patientIndex)
{
    FILE *file;

    file = fopen("patient_records.txt", "a");

    if(file == NULL)
    {
        printf("\nUnable to save patient record.\n");
        return;
    }

    fprintf(file, "========================================\n");
    fprintf(file, "Patient ID : PAT-%04d\n",
            1001 + patientIndex);
    fprintf(file, "Patient Name : %s\n",
            patientNames[patientIndex]);
    fprintf(file, "Age : %d\n",
            patientAges[patientIndex]);
    fprintf(file, "Urgency Level : %d\n",
            urgencyLevels[patientIndex]);
    fprintf(file, "Specialty : %s\n",
            specialtyNames[specialtyIDs[patientIndex] - 1]);

    fprintf(file, "Base Consultation Fee : LKR %.2f\n",
            baseFees[patientIndex]);

    fprintf(file, "Emergency Surcharge : LKR %.2f\n",
            surcharges[patientIndex]);

    fprintf(file, "Ward Stay Cost : LKR %.2f\n",
            wardCosts[patientIndex]);

    fprintf(file, "Gross Total : LKR %.2f\n",
            grossTotals[patientIndex]);

    fprintf(file, "Discount : LKR %.2f\n",
            discounts[patientIndex]);

    fprintf(file, "Final Payable Amount : LKR %.2f\n",
            finalBills[patientIndex]);

    fprintf(file, "Estimated Waiting Time : %.2f mins\n",
            waitingTimes[patientIndex]);

    fprintf(file, "========================================\n\n");

    fclose(file);
}
