#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define FILENAME "userData.txt"

struct userData
{
    int userId;
    char userName[100];
    int userAge;
};

int createFile()
{
    FILE *file = fopen(FILENAME, "a");
    if (file == NULL)
    {
        printf("Can't create file. please try again!");
        return -1;
    }
    fclose(file);
    return 0;
};

int duplicateID(int id){
    FILE *file = fopen(FILENAME, "r");
    if (file == NULL)
    {
        printf("error opening file! please try again!");
        return 0;
    }

    struct userData user;
    while(fscanf(file, "%d %s %d", &user.userId, user.userName, &user.userAge)==3){
        if(user.userId == id){
            fclose(file);
            return 1;
        }
    }
    fclose(file);
    return 0;

}

void createUser()
{
    FILE *file = fopen(FILENAME, "a");
    if (file == NULL)
    {
        printf("error opening file! please try again!");
        return;
    }

    struct userData newUser;

    printf("Enter Id:");
    scanf("%d", &newUser.userId);

    if(duplicateID(newUser.userId)==1){
        printf("user id already exists");
        fclose(file);
        return;
    }

    printf("Enter Name: ");
    scanf("%99s", newUser.userName);

    printf("Enter Age: ");
    scanf("%d", &newUser.userAge);

    fprintf(file, "%d %s %d\n", newUser.userId, newUser.userName, newUser.userAge);
    fclose(file);

    printf("New user added successfully!!\n");
}

void readUser()
{
    FILE *file = fopen(FILENAME, "r");
    if (file == NULL)
    {
        printf("error opening file!, please try again!");
        return;
    }

    struct userData readUser;
    int found = 0;

    while (fscanf(file, "%d %s %d", &readUser.userId, readUser.userName, &readUser.userAge) == 3)
    {
        printf("userID: %d, userName: %s, userAge: %d\n\n", readUser.userId, readUser.userName, readUser.userAge);
        found = 1;
    }

    if (found == 0)
    {
        printf("No users found! please create users first\n");
    }
    fclose(file);
}

void updateUser()
{
    FILE *file = fopen(FILENAME, "r");
    FILE *newFile = fopen("newFile.txt", "w");
    if (file == NULL || newFile == NULL)
    {
        printf("error opening file!, please try again!");
        return;
    }
    int idForUpdate;
    printf("Enter the userID you want to update: ");
    scanf("%d", &idForUpdate);

    struct userData updateUser;
    int found = 0;

    while (fscanf(file, "%d %s %d", &updateUser.userId, updateUser.userName, &updateUser.userAge) == 3)
    {
        if (updateUser.userId == idForUpdate)
        {
            printf("Enter New Name: ");
            scanf("%s", updateUser.userName);

            printf("Enter New Age: ");
            scanf("%d", &updateUser.userAge);
            found =1;
        }
        fprintf(newFile, "%d %s %d\n", updateUser.userId, updateUser.userName, updateUser.userAge);
    }
    fclose(newFile);
    fclose(file);
    remove(FILENAME);
    rename("newFile.txt", FILENAME);    

    if(found==0){
        printf("user with id %d not found", idForUpdate);
    } else{
        printf("User updated successfully\n");
    }
}

void deleteUser(){
     FILE *file = fopen(FILENAME, "r");
    FILE *newFile = fopen("newFile.txt", "w");
    if (file == NULL || newFile == NULL)
    {
        printf("error opening file!, please try again!");
        return;
    }

    int idForDelete;
    printf("Enter the userID you want to Delete: ");
    scanf("%d", &idForDelete);
    

    struct userData user;
    int found =0;

    while (fscanf(file, "%d %s %d", &user.userId, user.userName, &user.userAge) == 3)
    {
        if(user.userId== idForDelete){
              found = 1;
            continue;
        }
        fprintf(newFile, "%d %s %d\n", user.userId, user.userName, user.userAge);
    }
    fclose(file);
    fclose(newFile);
    remove(FILENAME);
    rename("newFile.txt", FILENAME);

    if(found ==0){
        printf("no user found with id %d", idForDelete);

    } else{
        printf("user deleted successfully\n");
    }

}


int main(){
    createFile();
    int choice;
    int true =1;

    while (true ==1)
    {
        printf("\nCRUD OPERATIONS\n");
        printf("1. Create New User\n");
        printf("2. Read All Users\n");
        printf("3. Update Any User\n");
        printf("4. Delete Any User\n");
        printf("5. Exit\n");
        printf("Enter choice: ");
        if(scanf("%d", &choice)!=1){
            printf("Please enter a number.\n");
            while (getchar() != '\n'); 
            continue;
        }


        switch (choice)
        {
        case 1:
            createUser();
            break;
        case 2:
            readUser();
            break;
        case 3:
            updateUser();
            break;
        case 4:
            deleteUser();
            break;
        case 5:
            printf("Thankyou!\n");
            exit(0);
        default:
            printf("please enter a valid choice.\n");
        }
    }
    return 0;
}
