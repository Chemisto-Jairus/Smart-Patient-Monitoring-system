# Smart Patient Monitoring and Record Management System

A console-based software system developed in **C** to help hospitals and clinics
monitor patient information and manage medical records efficiently.

---

## Features

| Menu Option | Description |
|---|---|
| 1. Add Patient | Register a new patient with demographics and vital signs |
| 2. View Patients | Display all stored patient records |
| 3. Search Patient | Find a patient by ID or by (partial) name |
| 4. Update Record | Edit any field of an existing patient record |
| 5. Delete Record | Remove a patient record (with confirmation prompt) |
| 6. Analyze Vital Signs | Flag abnormal temperature, heart rate, and blood pressure |
| 7. Save Data | Persist all records to `patients.dat` (binary file) |
| 8. Exit | Save and quit |

Data is automatically loaded from `patients.dat` on startup and saved on exit.

---

## Vital-Sign Normal Ranges

| Measurement | Normal Range |
|---|---|
| Temperature | 36.1 °C – 37.2 °C |
| Heart Rate | 60 – 100 bpm |
| Blood Pressure | 90–120 / 60–80 mmHg |

---

## Building

Requirements: **GCC** and **make**.

```bash
make          # compiles patient_monitoring
make clean    # removes binary and data file
```

## Running

```bash
./patient_monitoring
```

---

## C Concepts Applied

| Concept | Application |
|---|---|
| Variables & Data Types | Patient data storage |
| Structures (`struct`) | Patient record layout |
| Functions | Modular menu operations |
| Loops | Menu repetition |
| Selection statements | Decision making & validation |
| Arrays | In-memory patient list |
| Pointers | File I/O and record access |
| File Handling | Binary save/load (`fread`/`fwrite`) |
| Strings | Names, diagnoses, gender |
| Streams | `stdin`/`stdout` interaction |
