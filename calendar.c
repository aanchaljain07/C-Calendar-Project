#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Structure for storing events
typedef struct {
    int day, month, year;
    char details[100];
} Event;

// Function declarations
void displayCalendar(int year, int month);
int getDayNumber(int day, int month, int year);
int getDayInMonth(int month, int year);
void addEvent();
void viewEvents();
void deleteEvent();
int isLeapYear(int year);

// ================= MAIN MENU =================
int main() {
    int choice, year, month;
    while (1) {
        printf("\n===== CALENDAR PROJECT =====\n");
        printf("1. Display Calendar\n");
        printf("2. Add Event\n");
        printf("3. View Events\n");
        printf("4. Delete Event\n");
        printf("5. Exit\n");
        printf("Enter your choice: ");
        scanf("%d", &choice);

        switch (choice) {
            case 1:
                printf("Enter month and year (MM YYYY): ");
                scanf("%d %d", &month, &year);
                displayCalendar(year, month);
                break;
            case 2:
                addEvent();
                break;
            case 3:
                viewEvents();
                break;
            case 4:
                deleteEvent();
                break;
            case 5:
                exit(0);
            default:
                printf("Invalid choice! Try again.\n");
        }
    }
    return 0;
}

// ================= CALENDAR FUNCTIONS =================
int isLeapYear(int year) {
    return ((year % 4 == 0 && year % 100 != 0) || (year % 400 == 0));
}

int getDayInMonth(int month, int year) {
    int daysInMonth[] = {0,31,28,31,30,31,30,31,31,30,31,30,31};
    if (month == 2 && isLeapYear(year)) return 29;
    return daysInMonth[month];
}

int getDayNumber(int day, int month, int year) {
    if (month < 3) {
        month += 12;
        year -= 1;
    }
    int k = year % 100;
    int j = year / 100;
    int h = (day + (13 * (month + 1)) / 5 + k + (k / 4) + (j / 4) + (5 * j)) % 7;
    return ((h + 6) % 7); // Sunday = 0
}

void displayCalendar(int year, int month) {
    printf("\n\nCalendar - %02d/%d\n\n", month, year);
    printf("Sun Mon Tue Wed Thu Fri Sat\n");

    int days = getDayInMonth(month, year);
    int start = getDayNumber(1, month, year);

    for (int i = 0; i < start; i++) printf("    ");
    for (int d = 1; d <= days; d++) {
        printf("%3d ", d);
        if ((d + start) % 7 == 0) printf("\n");
    }
    printf("\n");
}

// ================= FILE HANDLING =================
void addEvent() {
    FILE *fp = fopen("events.dat", "ab");
    if (!fp) {
        printf("Error opening file!\n");
        return;
    }
    Event e;
    printf("Enter event date (DD MM YYYY): ");
    scanf("%d %d %d", &e.day, &e.month, &e.year);
    getchar(); // clear newline
    printf("Enter event details: ");
    fgets(e.details, 100, stdin);
    e.details[strcspn(e.details, "\n")] = 0; // remove newline
    fwrite(&e, sizeof(Event), 1, fp);
    fclose(fp);
    printf("Event added successfully!\n");
}

void viewEvents() {
    FILE *fp = fopen("events.dat", "rb");
    if (!fp) {
        printf("No events found!\n");
        return;
    }
    Event e;
    printf("\n==== All Events ====\n");
    while (fread(&e, sizeof(Event), 1, fp)) {
        printf("%02d-%02d-%d : %s\n", e.day, e.month, e.year, e.details);
    }
    fclose(fp);
}

void deleteEvent() {
    FILE *fp = fopen("events.dat", "rb");
    FILE *temp = fopen("temp.dat", "wb");
    if (!fp || !temp) {
        printf("File error!\n");
        return;
    }
    int d, m, y;
    printf("Enter event date to delete (DD MM YYYY): ");
    scanf("%d %d %d", &d, &m, &y);
    Event e;
    int found = 0;

    while (fread(&e, sizeof(Event), 1, fp)) {
        if (e.day == d && e.month == m && e.year == y) {
            found = 1; // skip writing
        } else {
            fwrite(&e, sizeof(Event), 1, temp);
        }
    }
    fclose(fp);
    fclose(temp);

    remove("events.dat");
    rename("temp.dat", "events.dat");

    if (found) printf("Event deleted successfully.\n");
    else printf("Event not found.\n");
}
