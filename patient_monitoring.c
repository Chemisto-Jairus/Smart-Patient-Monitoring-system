/*
 * Smart Patient Monitoring and Record Management System
 * Developed in C for console-based patient data management.
 *
 * Features:
 *   1. Add Patient
 *   2. View Patients
 *   3. Search Patient
 *   4. Update Record
 *   5. Delete Record
 *   6. Analyze Vital Signs
 *   7. Save Data
 *   8. Exit
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* --- Configuration and Struct --- */
#define MAX_P 100
#define DATA_FILE "patients.dat"

typedef struct {
    int id, age, hr, sbp, dbp;
    char name[50], gender[10], diag[100];
    float temp;
} Patient;

/* --- Global State --- */
Patient pts[MAX_P];
int count = 0, nxt_id = 1;

/* Helper to clear input buffer and prevent scanf skipping */
void clear() { while (getchar() != '\n'); }

/* Save data to binary file */
void save() {
    FILE *fp = fopen(DATA_FILE, "wb");
    if (fp) {
        fwrite(&count, sizeof(int), 1, fp);
        fwrite(&nxt_id, sizeof(int), 1, fp);
        fwrite(pts, sizeof(Patient), count, fp);
        fclose(fp);
        printf("Data saved successfully.\n");
    }
}

/* Load data from binary file on startup */
void load() {
    FILE *fp = fopen(DATA_FILE, "rb");
    if (fp && fread(&count, sizeof(int), 1, fp) && fread(&nxt_id, sizeof(int), 1, fp)) {
        fread(pts, sizeof(Patient), count, fp);
        fclose(fp);
    }
}

/* Concise display of patient record */
void print_p(Patient *p) {
    printf("\n[ID: %d] Name: %-15s Age: %d | %s\n", p->id, p->name, p->age, p->gender);
    printf("  Diag: %s\n", p->diag);
    printf("  Vitals: %.1fC, %d bpm, BP: %d/%d mmHg\n", p->temp, p->hr, p->sbp, p->dbp);
}

/* Add new patient with auto-incremented ID */
void add() {
    if (count >= MAX_P) { printf("System full!\n"); return; }
    Patient *p = &pts[count++];
    p->id = nxt_id++;

    printf("Name: "); fgets(p->name, 50, stdin); p->name[strcspn(p->name, "\n")] = 0;
    printf("Age: "); scanf("%d", &p->age); clear();
    printf("Gender: "); scanf("%s", p->gender); clear();
    printf("Diagnosis: "); fgets(p->diag, 100, stdin); p->diag[strcspn(p->diag, "\n")] = 0;
    
    // Quick entry for medical data
    printf("Enter Vitals (Temp HR SBP DBP): ");
    scanf("%f %d %d %d", &p->temp, &p->hr, &p->sbp, &p->dbp); clear();
}

/* Analyze signs against standard medical thresholds */
void analyze() {
    for (int i = 0; i < count; i++) {
        printf("\nAnalysis for %s:", pts[i].name);
        int abnormal = 0;
        if (pts[i].temp < 36.1 || pts[i].temp > 37.2) { printf(" [!] Temp"); abnormal = 1; }
        if (pts[i].hr < 60 || pts[i].hr > 100) { printf(" [!] Heart Rate"); abnormal = 1; }
        if (pts[i].sbp > 120 || pts[i].dbp > 80) { printf(" [!] BP High"); abnormal = 1; }
        
        if (!abnormal) printf(" All vitals normal.");
        printf("\n");
    }
}

int main() {
    int ch, id;
    load();
    
    while (1) {
        printf("\n1.Add 2.View 3.Search 4.Analyze 5.Delete 6.Exit\nChoice: ");
        if (scanf("%d", &ch) != 1) { clear(); continue; }
        clear();

        if (ch == 6) { save(); break; }
        
        switch(ch) {
            case 1: add(); break;
            case 2: 
                for(int i=0; i<count; i++) print_p(&pts[i]); 
                break;
            case 3: 
                printf("Enter ID: "); scanf("%d", &id); clear();
                for(int i=0; i<count; i++) if(pts[i].id == id) print_p(&pts[i]);
                break;
            case 4: analyze(); break;
            case 5: 
                printf("ID to delete: "); scanf("%d", &id); clear();
                for(int i=0; i<count; i++) if(pts[i].id == id) {
                    // Replace deleted item with the last item (Efficient swap-delete)
                    pts[i] = pts[--count]; 
                    printf("Record deleted.\n");
                }
                break;
            default: printf("Invalid choice.\n");
        }
    }
    return 0;
}