#include <stdio.h>
#include <string.h>
#include <stdlib.h> 

#define MAX 100

/* --- GLOBAL DATA (Parallel Arrays) --- */
int ids[MAX];
char names[MAX][50];
char diagnoses[MAX][100];
float temps[MAX];
int pulse[MAX];
int urgencyScore[MAX]; 
char priority[MAX][15]; 

// NEW ARRAYS FOR SCHEDULING
char assignedDoctor[MAX][50]; 
char appointmentTime[MAX][50];

/* --- FUNCTION PROTOTYPES --- */
void addPatient(int *total);
void searchByName(int total);
void sortByUrgency(int total);
void saveToFile(int total);
void loadFromFile(int *total);

int main() {
    int totalPatients = 0;
    int choice;

    loadFromFile(&totalPatients);

    while (1) {
        printf("\n======= MEDI-CORE MANAGEMENT SYSTEM =======\n");
        printf("1. Register, Triage & Assign Doctor\n");
        printf("2. Search Patient by Name\n");
        printf("3. Sort & View by Urgency (Critical First)\n");
        printf("4. View All Appointments & Records\n");
        printf("5. Save and Exit\n");
        printf("============================================\n");
        printf("Selection: ");
        
        if (scanf("%d", &choice) != 1) {
            while(getchar() != '\n'); 
            continue;
        }
        getchar(); 

        switch(choice) {
            case 1: addPatient(&totalPatients); break;
            case 2: searchByName(totalPatients); break;
            case 3: sortByUrgency(totalPatients); break;
            case 4: 
                if(totalPatients == 0) printf("\nNo records found.\n");
                for(int i = 0; i < totalPatients; i++) {
                    printf("\n[%s] ID:%d | Patient: %s | Diag: %s\n", priority[i], ids[i], names[i], diagnoses[i]);
                    printf(" -> Doctor: %s | Appt: %s\n", assignedDoctor[i], appointmentTime[i]);
                }
                break;
            case 5: saveToFile(totalPatients); return 0;
            default: printf("Invalid option.\n");
        }
    }
}

void addPatient(int *total) {
    if (*total >= MAX) {
        printf("Database full!\n");
        return;
    }
    static int idGen = 202601; 
    
    printf("Full Name: ");
    fgets(names[*total], 50, stdin);
    names[*total][strcspn(names[*total], "\n")] = 0;

    printf("Temp (C) & Pulse (BPM): ");
    scanf("%f %d", &temps[*total], &pulse[*total]);
    getchar();

    printf("Diagnosis: ");
    fgets(diagnoses[*total], 100, stdin);
    diagnoses[*total][strcspn(diagnoses[*total], "\n")] = 0;

    /* --- ENHANCED MEDICAL TRIAGE & SCHEDULING LOGIC --- */
    if (temps[*total] > 39.5 || pulse[*total] > 130) {
        strcpy(priority[*total], "CRITICAL");
        urgencyScore[*total] = 3;
        strcpy(assignedDoctor[*total], "Dr. Meredith Grey (Trauma)");
        strcpy(appointmentTime[*total], "IMMEDIATE (Transfer to ER)");

    } else if (temps[*total] > 37.8) {
        strcpy(priority[*total], "URGENT");
        urgencyScore[*total] = 2;
        strcpy(assignedDoctor[*total], "Dr. Gregory House (Diagnostics)");
        strcpy(appointmentTime[*total], "Within 2 Hours");

    } else {
        strcpy(priority[*total], "NORMAL");
        urgencyScore[*total] = 1;
        strcpy(assignedDoctor[*total], "Dr. John Dorian (General Practice)");
        strcpy(appointmentTime[*total], "Standard Slot (Within 3 Days)");
    }

    ids[*total] = idGen++;
    (*total)++;
    printf("Patient registered and assigned to %s.\n", assignedDoctor[*total-1]);
}

void searchByName(int total) {
    char query[50];
    int found = 0;
    printf("Enter name to search: ");
    fgets(query, 50, stdin);
    query[strcspn(query, "\n")] = 0;

    for(int i = 0; i < total; i++) {
        if(strstr(names[i], query)) {
            printf("\nFound: ID %d | %s | Status: %s\n", ids[i], names[i], priority[i]);
            printf("Assigned to: %s at %s\n", assignedDoctor[i], appointmentTime[i]);
            found = 1;
        }
    }
    if(!found) printf("No patient found.\n");
}

void sortByUrgency(int total) {
    for (int i = 0; i < total - 1; i++) {
        for (int j = 0; j < total - i - 1; j++) {
            if (urgencyScore[j] < urgencyScore[j + 1]) {
                // Swap IDs
                int tID = ids[j]; ids[j] = ids[j+1]; ids[j+1] = tID;
                // Swap Temps
                float tT = temps[j]; temps[j] = temps[j+1]; temps[j+1] = tT;
                // Swap Pulse
                int tP = pulse[j]; pulse[j] = pulse[j+1]; pulse[j+1] = tP;
                // Swap Scores
                int tS = urgencyScore[j]; urgencyScore[j] = urgencyScore[j+1]; urgencyScore[j+1] = tS;
                
                // Swap Strings
                char tStr[100]; 
                strcpy(tStr, names[j]); strcpy(names[j], names[j+1]); strcpy(names[j+1], tStr);
                strcpy(tStr, diagnoses[j]); strcpy(diagnoses[j], diagnoses[j+1]); strcpy(diagnoses[j+1], tStr);
                strcpy(tStr, priority[j]); strcpy(priority[j], priority[j+1]); strcpy(priority[j+1], tStr);
                
                // Swap New Doctor & Appointment Arrays
                strcpy(tStr, assignedDoctor[j]); strcpy(assignedDoctor[j], assignedDoctor[j+1]); strcpy(assignedDoctor[j+1], tStr);
                strcpy(tStr, appointmentTime[j]); strcpy(appointmentTime[j], appointmentTime[j+1]); strcpy(appointmentTime[j+1], tStr);
            }
        }
    }
    printf("Sorting complete. Critical patients and immediate appointments prioritized.\n");
}

void saveToFile(int total) {
    FILE *fp = fopen("hospital.txt", "w");
    if(fp == NULL) return;
    for(int i = 0; i < total; i++) {
        // Now saving 9 pieces of data separated by pipes
        fprintf(fp, "%d|%f|%d|%d|%s|%s|%s|%s|%s\n", 
            ids[i], temps[i], pulse[i], urgencyScore[i], 
            names[i], diagnoses[i], priority[i], 
            assignedDoctor[i], appointmentTime[i]);
    }
    fclose(fp);
    printf("Data saved successfully.\n");
}

void loadFromFile(int *total) {
    FILE *fp = fopen("hospital.txt", "r");
    if(fp == NULL) return;
    
    // Now reading 9 pieces of data. Notice the extra %[^|] tokens
    while(fscanf(fp, "%d|%f|%d|%d|%[^|]|%[^|]|%[^|]|%[^|]|%[^\n]\n", 
        &ids[*total], &temps[*total], &pulse[*total], &urgencyScore[*total], 
        names[*total], diagnoses[*total], priority[*total], 
        assignedDoctor[*total], appointmentTime[*total]) == 9) {
        (*total)++;
    }
    fclose(fp);
}
