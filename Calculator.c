#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>

#define MAX 100

void CheckExpression(const char *expression, int numbers[], char operators[], int *numberCount, int *operatorCount);
int evaluateExpression(int numbers[], char operators[], int numberCount, int operatorCount);

int main() {
    char expression[MAX];
    printf("Enter expression: ");
    fgets(expression, MAX, stdin);
    //variable names are changed
    int numbers[MAX];
    char operators[MAX];
    int numberCount = 0, operatorCount = 0;

    CheckExpression(expression, numbers, operators, &numberCount, &operatorCount);
    int result = evaluateExpression(numbers, operators, numberCount, operatorCount);
    printf("Result: %d\n", result);

    return 0;
}

//code logic is written seperately
void CheckExpression(const char *expression, int numbers[], char operators[], int *numberCount, int *operatorCount) {
    int i = 0;
    //flag made for checking invalid operator occurence
    int expectNumber = 1; 

    while (expression[i]) {
        if (isspace(expression[i])) {
            i++;
            continue;
        }

        if (isdigit(expression[i])) {
            int number = 0;
            while (isdigit(expression[i])) {
                number = number * 10 + (expression[i] - '0');
                i++;
            }
            numbers[(*numberCount)++] = number;
            //will tell that the number is seen 
            expectNumber = 0;
        } 
        else if (expression[i] == '+' || expression[i] == '-' || 
                 expression[i] == '*' || expression[i] == '/') {
            if (expectNumber) {
                printf("Error: Invalid operator placement near '%c'\n", expression[i]);
                exit(0);
            }
            operators[(*operatorCount)++] = expression[i];
            i++;
            //will tell theat the next should be number
            expectNumber = 1;
        } 
        else if (expression[i] == '\n') {
            break;
        } 
        else {
            printf("Error: Invalid character '%c' in expression.\n", expression[i]);
            exit(0);
        }
    }

    if (expectNumber) {
        printf("Error: Expression cannot end with an operator.\n");
        exit(0);
    }
}

int evaluateExpression(int numbers[], char operators[], int numberCount, int operatorCount) {
    for (int i = 0; i < operatorCount; i++) {
        if (operators[i] == '*' || operators[i] == '/') {
            if (operators[i] == '*') {
                numbers[i] *= numbers[i + 1];
            } else {
                if (numbers[i + 1] == 0) {
                    printf("Error: Division by zero.\n");
                    exit(0);
                }
                numbers[i] /= numbers[i + 1];
            }
            for (int j = i + 1; j < numberCount - 1; j++) {
                numbers[j] = numbers[j + 1];
            }
            for (int j = i; j < operatorCount - 1; j++) {
                operators[j] = operators[j + 1];
            }

            numberCount--;
            operatorCount--;
            i--;
        }
    }
    int result = numbers[0];
    for (int i = 0; i < operatorCount; i++) {
        if (operators[i] == '+') {
            result += numbers[i + 1];
        } else {
            result -= numbers[i + 1];
        }
    }

    return result;
}
