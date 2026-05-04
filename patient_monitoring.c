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

/* ─── Constants ─────────────────────────────────────────────────────────── */
#define MAX_PATIENTS   100
#define NAME_LEN        50
#define GENDER_LEN      10
#define DIAGNOSIS_LEN  100
#define DATA_FILE      "patients.dat"

/* Normal vital-sign ranges */
#define TEMP_LOW      36.1f
#define TEMP_HIGH     37.2f
#define HR_LOW        60
#define HR_HIGH       100
#define SBP_LOW       90
#define SBP_HIGH     120
#define DBP_LOW       60
#define DBP_HIGH      80

/* ─── Data Structures ────────────────────────────────────────────────────── */
typedef struct {
    int   id;
    char  name[NAME_LEN];
    int   age;
    char  gender[GENDER_LEN];
    char  diagnosis[DIAGNOSIS_LEN];
    float temperature;   /* °C */
    int   heart_rate;    /* bpm */
    int   systolic_bp;   /* mmHg */
    int   diastolic_bp;  /* mmHg */
} Patient;

/* ─── Globals ────────────────────────────────────────────────────────────── */
static Patient patients[MAX_PATIENTS];
static int     patient_count = 0;
static int     next_id       = 1;

/* ─── Forward Declarations ───────────────────────────────────────────────── */
void add_patient(void);
void view_patients(void);
void search_patient(void);
void update_patient(void);
void delete_patient(void);
void analyze_vital_signs(void);
void save_data(void);
void load_data(void);
void print_patient(const Patient *p);
void generate_report(void);
int  find_patient_by_id(int id);
void clear_input_buffer(void);

/* ─── Main ───────────────────────────────────────────────────────────────── */
int main(void)
{
    int choice;

    load_data();

    printf("╔══════════════════════════════════════════════════╗\n");
    printf("║   Smart Patient Monitoring & Record Management   ║\n");
    printf("╚══════════════════════════════════════════════════╝\n");

    do {
        printf("\n┌─────────────── MAIN MENU ───────────────┐\n");
        printf("│  1. Add Patient                          │\n");
        printf("│  2. View Patients                        │\n");
        printf("│  3. Search Patient                       │\n");
        printf("│  4. Update Record                        │\n");
        printf("│  5. Delete Record                        │\n");
        printf("│  6. Analyze Vital Signs                  │\n");
        printf("│  7. Save Data                            │\n");
        printf("│  8. Exit                                 │\n");
        printf("└─────────────────────────────────────────┘\n");
        printf("Enter choice: ");

        if (scanf("%d", &choice) != 1) {
            clear_input_buffer();
            printf("Invalid input. Please enter a number.\n");
            choice = 0;
            continue;
        }
        clear_input_buffer();

        switch (choice) {
            case 1: add_patient();        break;
            case 2: view_patients();      break;
            case 3: search_patient();     break;
            case 4: update_patient();     break;
            case 5: delete_patient();     break;
            case 6: analyze_vital_signs(); break;
            case 7: save_data();          break;
            case 8: save_data();
                    printf("Data saved. Goodbye!\n");
                    break;
            default:
                printf("Invalid choice. Please try again.\n");
        }
    } while (choice != 8);

    return 0;
}

/* ─── Helper: clear stdin after scanf ───────────────────────────────────── */
void clear_input_buffer(void)
{
    int c;
    while ((c = getchar()) != '\n' && c != EOF)
        ;
}

/* ─── Helper: print a single patient record ─────────────────────────────── */
void print_patient(const Patient *p)
{
    printf("┌───────────────────────────────────────────────────┐\n");
    printf("│ Patient ID   : %-34d │\n", p->id);
    printf("│ Name         : %-34s │\n", p->name);
    printf("│ Age          : %-34d │\n", p->age);
    printf("│ Gender       : %-34s │\n", p->gender);
    printf("│ Diagnosis    : %-34s │\n", p->diagnosis);
    printf("│ Temperature  : %-33.1f°C│\n", p->temperature);
    printf("│ Heart Rate   : %-31d bpm│\n", p->heart_rate);
    printf("│ Blood Pressure: %d/%d mmHg%-20s │\n",
           p->systolic_bp, p->diastolic_bp, "");
    printf("└───────────────────────────────────────────────────┘\n");
}

/* ─── Find patient index by ID (returns -1 if not found) ─────────────────── */
int find_patient_by_id(int id)
{
    for (int i = 0; i < patient_count; i++) {
        if (patients[i].id == id)
            return i;
    }
    return -1;
}

/* ─── 1. Add Patient ─────────────────────────────────────────────────────── */
void add_patient(void)
{
    if (patient_count >= MAX_PATIENTS) {
        printf("Patient list is full (max %d).\n", MAX_PATIENTS);
        return;
    }

    Patient *p = &patients[patient_count];
    p->id = next_id++;

    printf("\n--- Add New Patient (ID: %d) ---\n", p->id);

    printf("Name: ");
    fgets(p->name, NAME_LEN, stdin);
    p->name[strcspn(p->name, "\n")] = '\0';

    printf("Age: ");
    while (scanf("%d", &p->age) != 1 || p->age <= 0 || p->age > 150) {
        clear_input_buffer();
        printf("Invalid age. Enter a value between 1 and 150: ");
    }
    clear_input_buffer();

    printf("Gender (Male/Female/Other): ");
    fgets(p->gender, GENDER_LEN, stdin);
    p->gender[strcspn(p->gender, "\n")] = '\0';

    printf("Diagnosis: ");
    fgets(p->diagnosis, DIAGNOSIS_LEN, stdin);
    p->diagnosis[strcspn(p->diagnosis, "\n")] = '\0';

    printf("Temperature (°C): ");
    while (scanf("%f", &p->temperature) != 1 ||
           p->temperature < 30.0f || p->temperature > 45.0f) {
        clear_input_buffer();
        printf("Invalid temperature. Enter a value between 30.0 and 45.0: ");
    }
    clear_input_buffer();

    printf("Heart Rate (bpm): ");
    while (scanf("%d", &p->heart_rate) != 1 ||
           p->heart_rate < 20 || p->heart_rate > 300) {
        clear_input_buffer();
        printf("Invalid heart rate. Enter a value between 20 and 300: ");
    }
    clear_input_buffer();

    printf("Systolic Blood Pressure (mmHg): ");
    while (scanf("%d", &p->systolic_bp) != 1 ||
           p->systolic_bp < 50 || p->systolic_bp > 250) {
        clear_input_buffer();
        printf("Invalid value. Enter between 50 and 250: ");
    }
    clear_input_buffer();

    printf("Diastolic Blood Pressure (mmHg): ");
    while (scanf("%d", &p->diastolic_bp) != 1 ||
           p->diastolic_bp < 30 || p->diastolic_bp > 150) {
        clear_input_buffer();
        printf("Invalid value. Enter between 30 and 150: ");
    }
    clear_input_buffer();

    patient_count++;
    printf("Patient added successfully (ID: %d).\n", p->id);
}

/* ─── 2. View All Patients ───────────────────────────────────────────────── */
void view_patients(void)
{
    if (patient_count == 0) {
        printf("No patients registered.\n");
        return;
    }
    printf("\n=== Patient List (%d total) ===\n", patient_count);
    for (int i = 0; i < patient_count; i++) {
        print_patient(&patients[i]);
    }
}

/* ─── 3. Search Patient ──────────────────────────────────────────────────── */
void search_patient(void)
{
    int sub_choice;
    printf("\nSearch by:\n  1. Patient ID\n  2. Name\nChoice: ");
    if (scanf("%d", &sub_choice) != 1) {
        clear_input_buffer();
        return;
    }
    clear_input_buffer();

    if (sub_choice == 1) {
        int id;
        printf("Enter Patient ID: ");
        if (scanf("%d", &id) != 1) {
            clear_input_buffer();
            return;
        }
        clear_input_buffer();
        int idx = find_patient_by_id(id);
        if (idx == -1)
            printf("Patient with ID %d not found.\n", id);
        else
            print_patient(&patients[idx]);

    } else if (sub_choice == 2) {
        char query[NAME_LEN];
        printf("Enter name (or partial name): ");
        fgets(query, NAME_LEN, stdin);
        query[strcspn(query, "\n")] = '\0';

        int found = 0;
        for (int i = 0; i < patient_count; i++) {
            if (strstr(patients[i].name, query) != NULL) {
                print_patient(&patients[i]);
                found++;
            }
        }
        if (!found)
            printf("No patients found matching \"%s\".\n", query);
    } else {
        printf("Invalid choice.\n");
    }
}

/* ─── 4. Update Patient Record ───────────────────────────────────────────── */
void update_patient(void)
{
    int id;
    printf("Enter Patient ID to update: ");
    if (scanf("%d", &id) != 1) {
        clear_input_buffer();
        return;
    }
    clear_input_buffer();

    int idx = find_patient_by_id(id);
    if (idx == -1) {
        printf("Patient with ID %d not found.\n", id);
        return;
    }

    Patient *p = &patients[idx];
    printf("Updating record for: %s (ID: %d)\n", p->name, p->id);
    printf("Press Enter to keep the current value.\n\n");

    /* Name */
    char buf[DIAGNOSIS_LEN];
    printf("Name [%s]: ", p->name);
    fgets(buf, NAME_LEN, stdin);
    buf[strcspn(buf, "\n")] = '\0';
    if (buf[0] != '\0')
        strncpy(p->name, buf, NAME_LEN - 1);

    /* Age */
    printf("Age [%d]: ", p->age);
    fgets(buf, sizeof(buf), stdin);
    buf[strcspn(buf, "\n")] = '\0';
    if (buf[0] != '\0') {
        int age = atoi(buf);
        if (age > 0 && age <= 150)
            p->age = age;
        else
            printf("Invalid age; keeping previous value.\n");
    }

    /* Gender */
    printf("Gender [%s]: ", p->gender);
    fgets(buf, GENDER_LEN, stdin);
    buf[strcspn(buf, "\n")] = '\0';
    if (buf[0] != '\0')
        strncpy(p->gender, buf, GENDER_LEN - 1);

    /* Diagnosis */
    printf("Diagnosis [%s]: ", p->diagnosis);
    fgets(buf, DIAGNOSIS_LEN, stdin);
    buf[strcspn(buf, "\n")] = '\0';
    if (buf[0] != '\0')
        strncpy(p->diagnosis, buf, DIAGNOSIS_LEN - 1);

    /* Temperature */
    printf("Temperature [%.1f]: ", p->temperature);
    fgets(buf, sizeof(buf), stdin);
    buf[strcspn(buf, "\n")] = '\0';
    if (buf[0] != '\0') {
        float temp = atof(buf);
        if (temp >= 30.0f && temp <= 45.0f)
            p->temperature = temp;
        else
            printf("Invalid temperature; keeping previous value.\n");
    }

    /* Heart rate */
    printf("Heart Rate [%d]: ", p->heart_rate);
    fgets(buf, sizeof(buf), stdin);
    buf[strcspn(buf, "\n")] = '\0';
    if (buf[0] != '\0') {
        int hr = atoi(buf);
        if (hr >= 20 && hr <= 300)
            p->heart_rate = hr;
        else
            printf("Invalid heart rate; keeping previous value.\n");
    }

    /* Systolic BP */
    printf("Systolic BP [%d]: ", p->systolic_bp);
    fgets(buf, sizeof(buf), stdin);
    buf[strcspn(buf, "\n")] = '\0';
    if (buf[0] != '\0') {
        int sbp = atoi(buf);
        if (sbp >= 50 && sbp <= 250)
            p->systolic_bp = sbp;
        else
            printf("Invalid value; keeping previous value.\n");
    }

    /* Diastolic BP */
    printf("Diastolic BP [%d]: ", p->diastolic_bp);
    fgets(buf, sizeof(buf), stdin);
    buf[strcspn(buf, "\n")] = '\0';
    if (buf[0] != '\0') {
        int dbp = atoi(buf);
        if (dbp >= 30 && dbp <= 150)
            p->diastolic_bp = dbp;
        else
            printf("Invalid value; keeping previous value.\n");
    }

    printf("Record updated successfully.\n");
}

/* ─── 5. Delete Patient Record ───────────────────────────────────────────── */
void delete_patient(void)
{
    int id;
    printf("Enter Patient ID to delete: ");
    if (scanf("%d", &id) != 1) {
        clear_input_buffer();
        return;
    }
    clear_input_buffer();

    int idx = find_patient_by_id(id);
    if (idx == -1) {
        printf("Patient with ID %d not found.\n", id);
        return;
    }

    char confirm;
    printf("Are you sure you want to delete %s (ID: %d)? (y/n): ",
           patients[idx].name, id);
    if (scanf(" %c", &confirm) != 1) {
        clear_input_buffer();
        return;
    }
    clear_input_buffer();

    if (confirm != 'y' && confirm != 'Y') {
        printf("Deletion cancelled.\n");
        return;
    }

    /* Shift remaining patients left */
    for (int i = idx; i < patient_count - 1; i++)
        patients[i] = patients[i + 1];

    patient_count--;
    printf("Patient ID %d deleted successfully.\n", id);
}

/* ─── 6. Analyze Vital Signs ─────────────────────────────────────────────── */
void analyze_vital_signs(void)
{
    if (patient_count == 0) {
        printf("No patients registered.\n");
        return;
    }

    printf("\n=== Vital Signs Analysis ===\n");
    int abnormal_count = 0;

    for (int i = 0; i < patient_count; i++) {
        Patient *p = &patients[i];
        int abnormal = 0;

        printf("\nPatient: %s (ID: %d)\n", p->name, p->id);

        /* Temperature analysis */
        if (p->temperature < TEMP_LOW) {
            printf("  [!] Temperature %.1f°C is LOW (normal: %.1f–%.1f°C)\n",
                   p->temperature, TEMP_LOW, TEMP_HIGH);
            abnormal = 1;
        } else if (p->temperature > TEMP_HIGH) {
            printf("  [!] Temperature %.1f°C is HIGH (normal: %.1f–%.1f°C)\n",
                   p->temperature, TEMP_LOW, TEMP_HIGH);
            abnormal = 1;
        } else {
            printf("  [✓] Temperature %.1f°C — Normal\n", p->temperature);
        }

        /* Heart rate analysis */
        if (p->heart_rate < HR_LOW) {
            printf("  [!] Heart Rate %d bpm is LOW (normal: %d–%d bpm)\n",
                   p->heart_rate, HR_LOW, HR_HIGH);
            abnormal = 1;
        } else if (p->heart_rate > HR_HIGH) {
            printf("  [!] Heart Rate %d bpm is HIGH (normal: %d–%d bpm)\n",
                   p->heart_rate, HR_LOW, HR_HIGH);
            abnormal = 1;
        } else {
            printf("  [✓] Heart Rate %d bpm — Normal\n", p->heart_rate);
        }

        /* Blood pressure analysis */
        if (p->systolic_bp < SBP_LOW || p->diastolic_bp < DBP_LOW) {
            printf("  [!] Blood Pressure %d/%d mmHg is LOW"
                   " (normal: %d–%d / %d–%d mmHg)\n",
                   p->systolic_bp, p->diastolic_bp,
                   SBP_LOW, SBP_HIGH, DBP_LOW, DBP_HIGH);
            abnormal = 1;
        } else if (p->systolic_bp > SBP_HIGH || p->diastolic_bp > DBP_HIGH) {
            printf("  [!] Blood Pressure %d/%d mmHg is HIGH"
                   " (normal: %d–%d / %d–%d mmHg)\n",
                   p->systolic_bp, p->diastolic_bp,
                   SBP_LOW, SBP_HIGH, DBP_LOW, DBP_HIGH);
            abnormal = 1;
        } else {
            printf("  [✓] Blood Pressure %d/%d mmHg — Normal\n",
                   p->systolic_bp, p->diastolic_bp);
        }

        if (abnormal)
            abnormal_count++;
    }

    printf("\n--- Summary ---\n");
    printf("Total patients     : %d\n", patient_count);
    printf("With abnormal signs: %d\n", abnormal_count);
    printf("With normal signs  : %d\n", patient_count - abnormal_count);
}

/* ─── 7. Save Data ───────────────────────────────────────────────────────── */
void save_data(void)
{
    FILE *fp = fopen(DATA_FILE, "wb");
    if (fp == NULL) {
        perror("Error opening file for writing");
        return;
    }

    /* Write metadata: count and next_id */
    fwrite(&patient_count, sizeof(int), 1, fp);
    fwrite(&next_id,       sizeof(int), 1, fp);
    /* Write patient array */
    fwrite(patients, sizeof(Patient), (size_t)patient_count, fp);

    fclose(fp);
    printf("Data saved to '%s' (%d patient(s)).\n", DATA_FILE, patient_count);
}

/* ─── Load Data (called at startup) ─────────────────────────────────────── */
void load_data(void)
{
    FILE *fp = fopen(DATA_FILE, "rb");
    if (fp == NULL) {
        /* No existing data file — start fresh */
        return;
    }

    if (fread(&patient_count, sizeof(int), 1, fp) != 1 ||
        fread(&next_id,       sizeof(int), 1, fp) != 1) {
        printf("Warning: could not read file header; starting fresh.\n");
        patient_count = 0;
        next_id       = 1;
        fclose(fp);
        return;
    }

    if (patient_count < 0 || patient_count > MAX_PATIENTS) {
        printf("Warning: data file appears corrupt; starting fresh.\n");
        patient_count = 0;
        next_id       = 1;
        fclose(fp);
        return;
    }

    size_t read = fread(patients, sizeof(Patient), (size_t)patient_count, fp);
    if ((int)read != patient_count) {
        printf("Warning: incomplete data file; loaded %d of %d record(s).\n",
               (int)read, patient_count);
        patient_count = (int)read;
    }

    fclose(fp);
    printf("Loaded %d patient record(s) from '%s'.\n", patient_count, DATA_FILE);
}
