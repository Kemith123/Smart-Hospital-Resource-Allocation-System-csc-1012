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

int patientCount = 0;
int specialtyQueue[4] = {0};

void displayMenu();
void registerPatient();

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
                printf("\nDisplay Bed Occupancy selected.\n");
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



/* Doctor specialties */
const char specialtyNames[4][30] = {"General Practice","Paediatrics","Cardiology","Neurology"};

const float consultationFees[4] = {1500.00,2500.00,4500.00,5000.00};

const char consultationTimes[4][30] = {"15 mins","20 mins","30 mins","30 mins"};

const int dailyPatientCaps[4] = {30,20,12,10};


/* Hospital wards */
const char wardNames[4][50] = {"General Ward","Paediatric Ward","Surgical Ward","ICU(Intensive Care Unit)"};

const float wardDailyRates[4] = {3000.00,6000.00,12000.00,25000.00};

const int wardCapacities[4] = {20,10,10,5};

/* Bed occupancy */
int bedOccupancy[4][20] = {0};



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

    patientCount++;

    printf("\nPatient registered successfully!\n");
}

