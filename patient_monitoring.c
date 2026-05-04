#include <stdio.h>
#include <string.h>
#include <stdlib.h> // Required for system functions

#define MAX 100

/* --- GLOBAL DATA --- */
int ids[MAX];
char names[MAX][50];
char diagnoses[MAX][100];
float temps[MAX];
int pulse[MAX];
int urgencyScore[MAX]; 
char priority[MAX][15]; 

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
        printf("1. Register & Triage Patient\n");
        printf("2. Search Patient by Name\n");
        printf("3. Sort & View by Urgency (Critical First)\n");
        printf("4. View All Records\n");
        printf("5. Save and Exit\n");
        printf("============================================\n");
        printf("Selection: ");
        
        if (scanf("%d", &choice) != 1) {
            while(getchar() != '\n'); // Clear buffer if user enters a letter
            continue;
        }
        getchar(); // Clean the newline

        switch(choice) {
            case 1: addPatient(&totalPatients); break;
            case 2: searchByName(totalPatients); break;
            case 3: sortByUrgency(totalPatients); break;
            case 4: 
                if(totalPatients == 0) printf("\nNo records found.\n");
                for(int i = 0; i < totalPatients; i++) 
                    printf("[%s] ID:%d | %-15s | %s\n", priority[i], ids[i], names[i], diagnoses[i]);
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

    /* MEDICAL TRIAGE LOGIC */
    if (temps[*total] > 39.5 || pulse[*total] > 130) {
        strcpy(priority[*total], "CRITICAL");
        urgencyScore[*total] = 3;
    } else if (temps[*total] > 37.8) {
        strcpy(priority[*total], "URGENT");
        urgencyScore[*total] = 2;
    } else {
        strcpy(priority[*total], "NORMAL");
        urgencyScore[*total] = 1;
    }

    ids[*total] = idGen++;
    (*total)++;
    printf("Patient added successfully!\n");
}

void searchByName(int total) {
    char query[50];
    int found = 0;
    printf("Enter name to search: ");
    fgets(query, 50, stdin);
    query[strcspn(query, "\n")] = 0;

    for(int i = 0; i < total; i++) {
        if(strstr(names[i], query)) {
            printf("Found: ID %d | %s | Status: %s | Diag: %s\n", ids[i], names[i], priority[i], diagnoses[i]);
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
                // Swap Names
                char tN[50]; strcpy(tN, names[j]); strcpy(names[j], names[j+1]); strcpy(names[j+1], tN);
                // Swap Diagnosis
                char tD[100]; strcpy(tD, diagnoses[j]); strcpy(diagnoses[j], diagnoses[j+1]); strcpy(diagnoses[j+1], tD);
                // Swap Priority
                char tPr[15]; strcpy(tPr, priority[j]); strcpy(priority[j], priority[j+1]); strcpy(priority[j+1], tPr);
            }
        }
    }
    printf("Sorting complete.\n");
}

void saveToFile(int total) {
    FILE *fp = fopen("hospital.txt", "w");
    if(fp == NULL) return;
    for(int i = 0; i < total; i++) {
        fprintf(fp, "%d|%f|%d|%d|%s|%s|%s\n", ids[i], temps[i], pulse[i], urgencyScore[i], names[i], diagnoses[i], priority[i]);
    }
    fclose(fp);
    printf("Data saved successfully.\n");
}

void loadFromFile(int *total) {
    FILE *fp = fopen("hospital.txt", "r");
    if(fp == NULL) return;
    while(fscanf(fp, "%d|%f|%d|%d|%[^|]|%[^|]|%[^\n]\n", &ids[*total], &temps[*total], &pulse[*total], &urgencyScore[*total], names[*total], diagnoses[*total], priority[*total]) == 7) {
        (*total)++;
    }
    fclose(fp);
}
