    #include <stdio.h>
    #include <stdlib.h>

    struct studentData
    {
        int rollNumber;
        char name[100];
        int marks[3];
    };

    enum Grade
    {
        A, B, C, D, F
    };

    int calculateTotal(int marks1, int marks2, int marks3);
    enum Grade calculateGrade(float average);
    void printPerformance(enum Grade grade);
    void listOfRollNumber(int totalStudents);
    char getGradeLetter(enum Grade grade);
    void inputStudentDetails(struct studentData *students, int totalStudents);
    void printStudentsDetails(struct studentData *students, int totalStudents);

    int main()
    {
        int TotalStudents;
        printf("enter the number of students: ");

        scanf("%d", &TotalStudents);
        struct studentData *students = malloc(TotalStudents * sizeof(struct studentData));

        inputStudentDetails(students, TotalStudents);
        printStudentsDetails(students, TotalStudents);

        listOfRollNumber(TotalStudents);
        free(students);
        return 0;
    }

    void inputStudentDetails(struct studentData *students, int totalStudents)
    {
        printf("enter the student details (roll number, name, marks1, marks2, marks3): ");

        for (int index = 0; index < totalStudents; index++)
        {
            scanf("%d %s %d %d %d",
                &students[index].rollNumber,
                students[index].name,
                &students[index].marks[0],
                &students[index].marks[1],
                &students[index].marks[2]);
        }
    }

    void printStudentsDetails(struct studentData *students, int totalStudents)
    {

        for (int index = 0; index < totalStudents; index++)
        {

            printf("roll Number: %d\n", students[index].rollNumber);
            printf("Name: %s\n", students[index].name);

            int totalMarks = calculateTotal(students[index].marks[0], students[index].marks[1], students[index].marks[2]);

            printf("Total: %d\n", totalMarks);

            float average = totalMarks / 3.0;
            printf("average: %f\n", average);

            enum Grade grade = calculateGrade(average);
            printf("Grade: %c\n", getGradeLetter(grade));
            printf("performance: ");
            printPerformance(grade);
        }
    }

    int calculateTotal(int marks1, int marks2, int marks3)
    {
        return marks1 + marks2 + marks3;
    }

    enum Grade calculateGrade(float average)
    {
        if (average >= 85)
        {
            return A;
        }
        else if (average >= 70)
        {
            return B;
        }
        else if (average >= 50)
        {
            return C;
        }
        else if (average >= 35)
        {
            return D;
        }
        else
        {
            return F;
        }
    }

    char getGradeLetter(enum Grade grade)
    {
        switch (grade)
        {
        case A:
            return 'A';
        case B:
            return 'B';
        case C:
            return 'C';
        case D:
            return 'D';
        case F:
            return 'F';
        default:
            return 'X';
        }
    }

    void printPerformance(enum Grade grade)
    {
        switch (grade)
        {
        case A:
            printf("*****\n");
            break;
        case B:
            printf("****\n");
            break;
        case C:
            printf("***\n");
            break;
        case D:
            printf("**\n");
            break;
        case F:
            break;
        }
    }

    void listOfRollNumber(int totalStudents)
    {
        if (totalStudents == 1)
        {
            printf("List of Roll numbers: ");
            printf("%d ", totalStudents);
            return;
        }
        listOfRollNumber(totalStudents - 1);

        printf("%d ", totalStudents);
    }
