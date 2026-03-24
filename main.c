/*
 * Project Title: Expense & Budget Tracker (Personal Finance)
 * Student Name : [YOUR NAME]
 * Register No  : [YOUR REG NO]
 * Course       : C Programming - DA1
 * Faculty      : Dr. Dinakaran M
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX 100
#define FILE_NAME "expenses.txt"
#define BUDGET_FILE "budget.txt"

/* ── Struct Definition ── */
typedef struct {
    int    id;
    char   date[15];      /* format: DD-MM-YYYY */
    char   category[30];  /* Food, Travel, Medical, Shopping, Other */
    float  amount;
    char   note[100];
} Expense;

/* ── Global Data ── */
Expense expenses[MAX];
int     count = 0;
float   monthlyBudget = 0.0;

/* ══════════════════════════════════════════
   FUNCTION DECLARATIONS
══════════════════════════════════════════ */
void  loadFromFile();
void  saveToFile();
void  loadBudget();
void  saveBudget();
void  addExpense();
void  viewAllExpenses();
void  viewMonthlyTotal();
void  categoryWiseTotal();
void  searchExpense();
void  checkBudget();
void  deleteExpense();
void  setBudget();
void  displayMenu();
int   isValidDate(char *date);
int   isValidAmount(float amount);

/* ══════════════════════════════════════════
   MAIN FUNCTION
══════════════════════════════════════════ */
int main() {
    loadFromFile();
    loadBudget();

    int choice;
    printf("\n===================================\n");
    printf("  EXPENSE & BUDGET TRACKER\n");
    printf("===================================\n");

    do {
        displayMenu();
        printf("Enter your choice: ");
        if (scanf("%d", &choice) != 1) {
            printf("Invalid input! Please enter a number.\n");
            while (getchar() != '\n'); /* clear buffer */
            choice = -1;
            continue;
        }
        getchar(); /* consume newline */

        switch (choice) {
            case 1: addExpense();       break;
            case 2: viewAllExpenses();  break;
            case 3: viewMonthlyTotal(); break;
            case 4: categoryWiseTotal();break;
            case 5: searchExpense();    break;
            case 6: checkBudget();      break;
            case 7: deleteExpense();    break;
            case 8: setBudget();        break;
            case 0:
                saveToFile();
                saveBudget();
                printf("\nData saved. Goodbye!\n");
                break;
            default:
                printf("Invalid choice! Enter 0-8.\n");
        }
    } while (choice != 0);

    return 0;
}

/* ══════════════════════════════════════════
   DISPLAY MENU
══════════════════════════════════════════ */
void displayMenu() {
    printf("\n-----------------------------------\n");
    printf(" 1. Add Expense\n");
    printf(" 2. View All Expenses\n");
    printf(" 3. Monthly Total\n");
    printf(" 4. Category-wise Total\n");
    printf(" 5. Search Expense\n");
    printf(" 6. Check Budget Status\n");
    printf(" 7. Delete Expense\n");
    printf(" 8. Set Monthly Budget\n");
    printf(" 0. Save & Exit\n");
    printf("-----------------------------------\n");
}

/* ══════════════════════════════════════════
   VALIDATION HELPERS
══════════════════════════════════════════ */
int isValidDate(char *date) {
    /* expects DD-MM-YYYY, length = 10 */
    if (strlen(date) != 10) return 0;
    if (date[2] != '-' || date[5] != '-') return 0;
    int day   = atoi(date);
    int month = atoi(date + 3);
    int year  = atoi(date + 6);
    if (day < 1 || day > 31)   return 0;
    if (month < 1 || month > 12) return 0;
    if (year < 2000 || year > 2100) return 0;
    return 1;
}

int isValidAmount(float amount) {
    return (amount > 0);
}

/* ══════════════════════════════════════════
   1. ADD EXPENSE
══════════════════════════════════════════ */
void addExpense() {
    if (count >= MAX) {
        printf("Storage full! Cannot add more expenses.\n");
        return;
    }

    Expense e;
    e.id = (count > 0) ? expenses[count - 1].id + 1 : 1;

    /* Date */
    do {
        printf("Enter date (DD-MM-YYYY): ");
        fgets(e.date, sizeof(e.date), stdin);
        e.date[strcspn(e.date, "\n")] = '\0';
        if (!isValidDate(e.date))
            printf("Invalid date format! Use DD-MM-YYYY.\n");
    } while (!isValidDate(e.date));

    /* Category */
    printf("Category (Food/Travel/Medical/Shopping/Other): ");
    fgets(e.category, sizeof(e.category), stdin);
    e.category[strcspn(e.category, "\n")] = '\0';
    if (strlen(e.category) == 0) strcpy(e.category, "Other");

    /* Amount */
    do {
        printf("Enter amount (Rs.): ");
        if (scanf("%f", &e.amount) != 1) {
            printf("Invalid amount!\n");
            while (getchar() != '\n');
            e.amount = -1;
            continue;
        }
        getchar();
        if (!isValidAmount(e.amount))
            printf("Amount must be greater than 0!\n");
    } while (!isValidAmount(e.amount));

    /* Note */
    printf("Enter note (optional, press Enter to skip): ");
    fgets(e.note, sizeof(e.note), stdin);
    e.note[strcspn(e.note, "\n")] = '\0';
    if (strlen(e.note) == 0) strcpy(e.note, "-");

    expenses[count++] = e;
    printf("Expense added successfully! (ID: %d)\n", e.id);

    /* Auto budget warning */
    checkBudget();
}

/* ══════════════════════════════════════════
   2. VIEW ALL EXPENSES
══════════════════════════════════════════ */
void viewAllExpenses() {
    if (count == 0) {
        printf("No expenses recorded yet.\n");
        return;
    }
    printf("\n%-5s %-12s %-12s %-10s %s\n",
           "ID", "Date", "Category", "Amount", "Note");
    printf("-----------------------------------------------------------\n");
    for (int i = 0; i < count; i++) {
        printf("%-5d %-12s %-12s Rs.%-7.2f %s\n",
               expenses[i].id,
               expenses[i].date,
               expenses[i].category,
               expenses[i].amount,
               expenses[i].note);
    }
    printf("-----------------------------------------------------------\n");
    printf("Total records: %d\n", count);
}

/* ══════════════════════════════════════════
   3. MONTHLY TOTAL
══════════════════════════════════════════ */
void viewMonthlyTotal() {
    char mm[3], yyyy[5];
    printf("Enter month (MM): ");
    fgets(mm, sizeof(mm), stdin);
    mm[strcspn(mm, "\n")] = '\0';
    printf("Enter year (YYYY): ");
    fgets(yyyy, sizeof(yyyy), stdin);
    yyyy[strcspn(yyyy, "\n")] = '\0';

    float total = 0.0;
    int   found = 0;

    printf("\nExpenses for %s-%s:\n", mm, yyyy);
    printf("%-5s %-12s %-12s %-10s %s\n",
           "ID", "Date", "Category", "Amount", "Note");
    printf("-----------------------------------------------------------\n");

    for (int i = 0; i < count; i++) {
        /* date format: DD-MM-YYYY, month at index 3, year at index 6 */
        char eMonth[3], eYear[5];
        strncpy(eMonth, expenses[i].date + 3, 2); eMonth[2] = '\0';
        strncpy(eYear,  expenses[i].date + 6, 4); eYear[4]  = '\0';

        if (strcmp(eMonth, mm) == 0 && strcmp(eYear, yyyy) == 0) {
            printf("%-5d %-12s %-12s Rs.%-7.2f %s\n",
                   expenses[i].id,
                   expenses[i].date,
                   expenses[i].category,
                   expenses[i].amount,
                   expenses[i].note);
            total += expenses[i].amount;
            found = 1;
        }
    }

    if (!found) {
        printf("No expenses found for %s-%s.\n", mm, yyyy);
        return;
    }
    printf("-----------------------------------------------------------\n");
    printf("Total for %s-%s: Rs.%.2f\n", mm, yyyy, total);

    if (monthlyBudget > 0) {
        if (total > monthlyBudget)
            printf("WARNING: You have EXCEEDED your budget of Rs.%.2f by Rs.%.2f!\n",
                   monthlyBudget, total - monthlyBudget);
        else
            printf("You are within budget. Remaining: Rs.%.2f\n",
                   monthlyBudget - total);
    }
}

/* ══════════════════════════════════════════
   4. CATEGORY-WISE TOTAL
══════════════════════════════════════════ */
void categoryWiseTotal() {
    if (count == 0) {
        printf("No expenses recorded yet.\n");
        return;
    }

    /* collect unique categories */
    char  cats[MAX][30];
    float totals[MAX];
    int   catCount = 0;

    for (int i = 0; i < count; i++) {
        int found = 0;
        for (int j = 0; j < catCount; j++) {
            if (strcasecmp(cats[j], expenses[i].category) == 0) {
                totals[j] += expenses[i].amount;
                found = 1;
                break;
            }
        }
        if (!found) {
            strcpy(cats[catCount], expenses[i].category);
            totals[catCount] = expenses[i].amount;
            catCount++;
        }
    }

    printf("\n%-20s %s\n", "Category", "Total Amount");
    printf("-----------------------------------\n");
    float grandTotal = 0;
    for (int i = 0; i < catCount; i++) {
        printf("%-20s Rs.%.2f\n", cats[i], totals[i]);
        grandTotal += totals[i];
    }
    printf("-----------------------------------\n");
    printf("%-20s Rs.%.2f\n", "GRAND TOTAL", grandTotal);
}

/* ══════════════════════════════════════════
   5. SEARCH EXPENSE
══════════════════════════════════════════ */
void searchExpense() {
    int opt;
    printf("Search by:\n 1. Date\n 2. Category\nChoice: ");
    scanf("%d", &opt);
    getchar();

    char keyword[30];
    int  found = 0;

    if (opt == 1) {
        printf("Enter date (DD-MM-YYYY): ");
        fgets(keyword, sizeof(keyword), stdin);
        keyword[strcspn(keyword, "\n")] = '\0';
        printf("\nResults for date: %s\n", keyword);
        printf("%-5s %-12s %-12s %-10s %s\n",
               "ID", "Date", "Category", "Amount", "Note");
        printf("-----------------------------------------------------------\n");
        for (int i = 0; i < count; i++) {
            if (strcmp(expenses[i].date, keyword) == 0) {
                printf("%-5d %-12s %-12s Rs.%-7.2f %s\n",
                       expenses[i].id, expenses[i].date,
                       expenses[i].category, expenses[i].amount,
                       expenses[i].note);
                found = 1;
            }
        }
    } else if (opt == 2) {
        printf("Enter category: ");
        fgets(keyword, sizeof(keyword), stdin);
        keyword[strcspn(keyword, "\n")] = '\0';
        printf("\nResults for category: %s\n", keyword);
        printf("%-5s %-12s %-12s %-10s %s\n",
               "ID", "Date", "Category", "Amount", "Note");
        printf("-----------------------------------------------------------\n");
        for (int i = 0; i < count; i++) {
            if (strcasecmp(expenses[i].category, keyword) == 0) {
                printf("%-5d %-12s %-12s Rs.%-7.2f %s\n",
                       expenses[i].id, expenses[i].date,
                       expenses[i].category, expenses[i].amount,
                       expenses[i].note);
                found = 1;
            }
        }
    } else {
        printf("Invalid option.\n");
        return;
    }

    if (!found) printf("No matching expenses found.\n");
}

/* ══════════════════════════════════════════
   6. CHECK BUDGET
══════════════════════════════════════════ */
void checkBudget() {
    if (monthlyBudget <= 0) {
        printf("\nNo monthly budget set. Use option 8 to set one.\n");
        return;
    }

    /* get current month-year from first matching entry, or ask user */
    char mm[3], yyyy[5];
    printf("Enter month to check (MM): ");
    fgets(mm, sizeof(mm), stdin);
    mm[strcspn(mm, "\n")] = '\0';
    printf("Enter year (YYYY): ");
    fgets(yyyy, sizeof(yyyy), stdin);
    yyyy[strcspn(yyyy, "\n")] = '\0';

    float total = 0.0;
    for (int i = 0; i < count; i++) {
        char eMonth[3], eYear[5];
        strncpy(eMonth, expenses[i].date + 3, 2); eMonth[2] = '\0';
        strncpy(eYear,  expenses[i].date + 6, 4); eYear[4]  = '\0';
        if (strcmp(eMonth, mm) == 0 && strcmp(eYear, yyyy) == 0)
            total += expenses[i].amount;
    }

    printf("\n--- Budget Status for %s-%s ---\n", mm, yyyy);
    printf("Monthly Budget : Rs.%.2f\n", monthlyBudget);
    printf("Total Spent    : Rs.%.2f\n", total);
    printf("Remaining      : Rs.%.2f\n", monthlyBudget - total);

    if (total > monthlyBudget)
        printf("STATUS: *** BUDGET EXCEEDED by Rs.%.2f! ***\n",
               total - monthlyBudget);
    else if (total >= monthlyBudget * 0.9)
        printf("STATUS: WARNING - You have used 90%% of your budget!\n");
    else
        printf("STATUS: Within budget.\n");
}

/* ══════════════════════════════════════════
   7. DELETE EXPENSE
══════════════════════════════════════════ */
void deleteExpense() {
    if (count == 0) {
        printf("No expenses to delete.\n");
        return;
    }
    int id;
    printf("Enter Expense ID to delete: ");
    scanf("%d", &id);
    getchar();

    for (int i = 0; i < count; i++) {
        if (expenses[i].id == id) {
            for (int j = i; j < count - 1; j++)
                expenses[j] = expenses[j + 1];
            count--;
            printf("Expense ID %d deleted successfully.\n", id);
            return;
        }
    }
    printf("Expense ID %d not found.\n", id);
}

/* ══════════════════════════════════════════
   8. SET MONTHLY BUDGET
══════════════════════════════════════════ */
void setBudget() {
    printf("Enter monthly budget (Rs.): ");
    if (scanf("%f", &monthlyBudget) != 1 || monthlyBudget <= 0) {
        printf("Invalid budget amount!\n");
        monthlyBudget = 0;
        while (getchar() != '\n');
        return;
    }
    getchar();
    printf("Monthly budget set to Rs.%.2f\n", monthlyBudget);
    saveBudget();
}

/* ══════════════════════════════════════════
   FILE HANDLING — SAVE & LOAD
══════════════════════════════════════════ */
void saveToFile() {
    FILE *fp = fopen(FILE_NAME, "w");
    if (!fp) {
        printf("Error: Could not open file for saving.\n");
        return;
    }
    fprintf(fp, "%d\n", count);
    for (int i = 0; i < count; i++) {
        fprintf(fp, "%d|%s|%s|%.2f|%s\n",
                expenses[i].id,
                expenses[i].date,
                expenses[i].category,
                expenses[i].amount,
                expenses[i].note);
    }
    fclose(fp);
    printf("Data saved to %s\n", FILE_NAME);
}

void loadFromFile() {
    FILE *fp = fopen(FILE_NAME, "r");
    if (!fp) return; /* file doesn't exist yet — first run */

    fscanf(fp, "%d\n", &count);
    for (int i = 0; i < count; i++) {
        fscanf(fp, "%d|%14[^|]|%29[^|]|%f|%99[^\n]\n",
               &expenses[i].id,
               expenses[i].date,
               expenses[i].category,
               &expenses[i].amount,
               expenses[i].note);
    }
    fclose(fp);
}

void saveBudget() {
    FILE *fp = fopen(BUDGET_FILE, "w");
    if (!fp) return;
    fprintf(fp, "%.2f\n", monthlyBudget);
    fclose(fp);
}

void loadBudget() {
    FILE *fp = fopen(BUDGET_FILE, "r");
    if (!fp) return;
    fscanf(fp, "%f", &monthlyBudget);
    fclose(fp);
}