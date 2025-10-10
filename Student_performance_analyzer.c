#include <stdio.h>
#include <stdlib.h>

struct studentData{
    int rollNumber;
    char name[100];
    int marks[3];
};


int calculateTotal(int marks1, int marks2, int marks3);
char calculateGrade(float average);
void printPerformance(char grade);
void listOfRollNumber(int totalStudents);

int main()
{
    int TotalStudents;
    printf("enter the number of students: ");
    
    scanf("%d", &TotalStudents);
    struct studentData *students = malloc(TotalStudents * sizeof(struct studentData));

    printf("enter the student details (roll number, name, marks1, marks2, marks3): ");

    for(int index=0;index<TotalStudents;index++){
        scanf("%d %s %d %d %d",
        &students[index].rollNumber,
        students[index].name,
        &students[index].marks[0],
         &students[index].marks[1],
         &students[index].marks[2]);
    }

    for (int index = 0; index < TotalStudents; index++) {

        printf("roll Number: %d\n", students[index].rollNumber);
        printf("Name: %s\n", students[index].name);


        int totalMarks = calculateTotal(students[index].marks[0],students[index].marks[1],students[index].marks[2]);

        printf("Total: %d\n", totalMarks);

        float average = totalMarks/3.0;
        printf("average: %f\n", average);

        char grade = calculateGrade(average);
        printf("grade: %c\n", grade);
        printf("performance: ");
        printPerformance(grade);
    }

    listOfRollNumber(TotalStudents);
    free(students);
    return 0;
}

int calculateTotal(int marks1, int marks2, int marks3){
    return marks1+ marks2+ marks3;
}

char calculateGrade(float average){
    if(average>=85){
        return 'A';
    } else if( average>= 70){
        return 'B';
    } else if( average>= 50){
        return 'C';
    } else if( average>= 35){
        return 'D';
    } else {
        return 'F';
    }
}

void printPerformance(char grade){
    if(grade=='A')
    {
        printf("*****\n");
        return;
    } else if (grade=='B')
    {
         printf("****\n");
         return;
    } else if (grade=='C')
    {
         printf("***\n");
         return;
    } else if (grade=='D')
    {
         printf("**\n");
         return;
    } else{
        return;
    }
    
}

void listOfRollNumber(int totalStudents){
    if(totalStudents ==1){
        printf("List of Roll numbers: ");
        printf("%d ",totalStudents);
        return;
    }
    listOfRollNumber(totalStudents-1);

    printf("%d ",totalStudents);
}