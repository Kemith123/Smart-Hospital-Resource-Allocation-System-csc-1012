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

int main()
{
    int choice;

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
                printf("\nDisplay Patient Priority selected.\n");
                break;

            case 4:
                printf("\nGenerate Reports selected.\n");
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

    
waitingTimes[patientCount] = calculateWaitingTime(specialtyIDs[patientCount]);
patientCount++;

if(admittedToWard[patientCount - 1] == 1)
   {
    allocateBed(patientCount - 1);
   }

printf("\nPatient registered successfully!\n");
    
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
