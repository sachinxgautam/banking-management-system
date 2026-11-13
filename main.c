#include <stdio.h>
#include <string.h>

struct Account {
    int accNo;
    char name[50];
    float balance;
};

struct Transaction {
    int accNo;
    char type[20];
    float amount;
};

void createAccount() {
    struct Account a, temp;
    FILE *fp = fopen("accounts.dat", "ab+");

    if (fp == NULL) {
        printf("File error!\n");
        return;
    }

    printf("Enter Account Number: ");
    scanf("%d", &a.accNo);

    rewind(fp);
    while (fread(&temp, sizeof(temp), 1, fp) == 1) {
        if (temp.accNo == a.accNo) {
            printf("Account already exists!\n");
            fclose(fp);
            return;
        }
    }

    printf("Enter Name: ");
    scanf(" %49[^\n]", a.name);

    printf("Enter Initial Balance: ");
    scanf("%f", &a.balance);

    if (a.balance < 0) {
        printf("Invalid balance!\n");
        fclose(fp);
        return;
    }

    fwrite(&a, sizeof(a), 1, fp);
    fclose(fp);

    printf("Account created successfully!\n");
}

void viewAccounts() {
    struct Account a;
    FILE *fp = fopen("accounts.dat", "rb");

    if (fp == NULL) {
        printf("No accounts found!\n");
        return;
    }

    printf("\n===== ACCOUNT DETAILS =====\n");

    while (fread(&a, sizeof(a), 1, fp) == 1) {
        printf("\nAccount No: %d\n", a.accNo);
        printf("Name: %s\n", a.name);
        printf("Balance: Rs. %.2f\n", a.balance);
    }

    fclose(fp);
}

void searchAccount() {
    struct Account a;
    int number, found = 0;

    FILE *fp = fopen("accounts.dat", "rb");

    if (fp == NULL) {
        printf("No accounts found!\n");
        return;
    }

    printf("Enter Account Number: ");
    scanf("%d", &number);

    while (fread(&a, sizeof(a), 1, fp) == 1) {
        if (a.accNo == number) {
            printf("\nAccount Found!\n");
            printf("Name: %s\n", a.name);
            printf("Balance: Rs. %.2f\n", a.balance);
            found = 1;
            break;
        }
    }

    if (!found)
        printf("Account not found!\n");

    fclose(fp);
}

void transaction(int type) {
    struct Account a;
    struct Transaction t;
    int number, found = 0;
    float amount;

    FILE *fp = fopen("accounts.dat", "rb+");

    if (fp == NULL) {
        printf("No accounts found!\n");
        return;
    }

    printf("Enter Account Number: ");
    scanf("%d", &number);

    while (fread(&a, sizeof(a), 1, fp) == 1) {
        if (a.accNo == number) {
            found = 1;

            printf("Current Balance: Rs. %.2f\n", a.balance);
            printf("Enter Amount: ");
            scanf("%f", &amount);

            if (amount <= 0) {
                printf("Invalid amount!\n");
                fclose(fp);
                return;
            }

            if (type == 1) {
                a.balance += amount;
                strcpy(t.type, "Deposit");
            } else {
                if (amount > a.balance) {
                    printf("Insufficient balance!\n");
                    fclose(fp);
                    return;
                }

                a.balance -= amount;
                strcpy(t.type, "Withdraw");
            }

            fseek(fp, -(long)sizeof(a), SEEK_CUR);
            fwrite(&a, sizeof(a), 1, fp);

            t.accNo = number;
            t.amount = amount;

            FILE *tf = fopen("transactions.dat", "ab");
            if (tf != NULL) {
                fwrite(&t, sizeof(t), 1, tf);
                fclose(tf);
            }

            printf("Transaction successful!\n");
            printf("New Balance: Rs. %.2f\n", a.balance);
            break;
        }
    }

    if (!found)
        printf("Account not found!\n");

    fclose(fp);
}

void history() {
    struct Transaction t;
    int number, found = 0;

    FILE *fp = fopen("transactions.dat", "rb");

    if (fp == NULL) {
        printf("No transaction history!\n");
        return;
    }

    printf("Enter Account Number: ");
    scanf("%d", &number);

    printf("\n===== TRANSACTION HISTORY =====\n");
    while (fread(&t, sizeof(t), 1, fp) == 1) {
        if (t.accNo == number) {
            printf("%s: Rs. %.2f\n", t.type, t.amount);
            found = 1;
        }
    }

    if (!found)
        printf("No transactions found!\n");

    fclose(fp);
}

int main() {
    int choice;

    while (1) {
        printf("\n===== BANK MANAGEMENT SYSTEM =====\n");
        printf("1. Create Account\n");
        printf("2. View Accounts\n");
        printf("3. Search Account\n");
        printf("4. Deposit Money\n");
        printf("5. Withdraw Money\n");
        printf("6. Transaction History\n");
        printf("7. Exit\n");

        printf("Enter your choice: ");

        if (scanf("%d", &choice) != 1) {
            printf("Invalid input!\n");
            return 1;
        }

        switch (choice) {
            case 1:
                createAccount();
                break;
            case 2:
                viewAccounts();
                break;
            case 3:
                searchAccount();
                break;
            case 4:
                transaction(1);
                break;
            case 5:
                transaction(2);
                break;
            case 6:
                history();
                break;
            case 7:
                printf("Thank you for using the system!\n");
                return 0;
            default:
                printf("Invalid choice!\n");
        }
    }

    return 0;
}