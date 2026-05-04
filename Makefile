CC      = gcc
CFLAGS  = -Wall -Wextra -pedantic -std=c11
TARGET  = patient_monitoring
SRC     = patient_monitoring.c

.PHONY: all clean

all: $(TARGET)

$(TARGET): $(SRC)
	$(CC) $(CFLAGS) -o $(TARGET) $(SRC)

clean:
	rm -f $(TARGET) patients.dat
