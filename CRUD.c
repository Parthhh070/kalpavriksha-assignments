#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define FILENAME "users.txt"

struct User {
    int id;
    char name[50];
    int age;
};

void initFile() {
    FILE *file = fopen(FILENAME, "a");
    if (file == NULL) {
        printf("Error creating file!\n");
        exit(1);
    }
    fclose(file);
}

void createUser() {
    FILE *file = fopen(FILENAME, "a");
    if (!file) {
        printf("Error opening file!\n");
        return;
    }

    struct User user;
  

    printf("Enter Id:");
    scanf("%d" , user.id);

    printf("Enter Name: ");
    scanf("%s", user.name);

    printf("Enter Age: ");
    scanf("%d", &user.age);


    fprintf(file, "%d %s %d\n", user.id, user.name, user.age);
    fclose(file);

    printf("User added successfully!\n");
}

void readUsers() {
    FILE *file = fopen(FILENAME, "r");
    if (!file) {
        printf("Error opening file!\n");
        return;
    }

    struct User user;
    printf("\n--- All Users ---\n");
    int found = 0;

    while (fscanf(file, "%d %s %d", &user.id, user.name, &user.age) == 3) {
        printf("ID: %d, Name: %s, Age: %d\n", user.id, user.name, user.age);
        found = 1;
    }

    if (!found) {
        printf("No users found!\n");
    }

    fclose(file);
}


void updateUser() {
    FILE *file = fopen(FILENAME, "r");
    FILE *temp = fopen("temp.txt", "w");

    if (!file || !temp) {
        printf("Error opening file!\n");
        return;
    }

    int id;
    printf("Enter User ID to update: ");
    scanf("%d", &id);

    struct User user;
    int found = 0;

    while (fscanf(file, "%d %s %d", &user.id, user.name, &user.age) == 3) {
        if (user.id == id) {
            printf("Enter new Name: ");
            scanf("%s", user.name);
            printf("Enter new Age: ");
            scanf("%d", &user.age);
            found = 1;
        }
        fprintf(temp, "%d %s %d\n", user.id, user.name, user.age);
    }

    fclose(file);
    fclose(temp);

    remove(FILENAME);
    rename("temp.txt", FILENAME);

    if (found)
        printf("User updated successfully!\n");
    else
        printf("User not found!\n");
}

void deleteUser() {
    FILE *file = fopen(FILENAME, "r");
    FILE *temp = fopen("temp.txt", "w");

    if (!file || !temp) {
        printf("Error opening file!\n");
        return;
    }

    int id;
    printf("Enter User ID to delete: ");
    scanf("%d", &id);

    struct User user;
    int found = 0;

    while (fscanf(file, "%d %s %d", &user.id, user.name, &user.age) == 3) {
        if (user.id == id) {
            found = 1;
            continue; 
        }
        fprintf(temp, "%d %s %d\n", user.id, user.name, user.age);
    }

    fclose(file);
    fclose(temp);

    remove(FILENAME);
    rename("temp.txt", FILENAME);

    if (found)
        printf("User deleted successfully!\n");
    else
        printf("User not found!\n");
}

int main() {
    initFile();
    int choice;

    while (1) {
        printf("\n===== User Management =====\n");
        printf("1. Create User\n");
        printf("2. Read Users\n");
        printf("3. Update User\n");
        printf("4. Delete User\n");
        printf("5. Exit\n");
        printf("Enter choice: ");
        scanf("%d", &choice);

        switch (choice) {
            case 1: createUser(); break;
            case 2: readUsers(); break;
            case 3: updateUser(); break;
            case 4: deleteUser(); break;
            case 5: printf("Exiting...\n"); exit(0);
            default: printf("Invalid choice! Try again.\n");
        }
    }
}
