#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>

#define MAX 100


void CheckExpression(const char *expression, int numbers[], char operators[], int *numCount, int *opCount);
int evaluateExpression(int numbers[], char operators[], int numCount, int opCount);

int main() {
    char expression[MAX];
    printf("Enter expression: ");
    fgets(expression, MAX, stdin);

    int numbers[MAX];
    char operators[MAX];
    int numCount = 0, opCount = 0;

    
    CheckExpression(expression, numbers, operators, &numCount, &opCount);

    int result = evaluateExpression(numbers, operators, numCount, opCount);
    printf("Result: %d\n", result);

    return 0;
}

void CheckExpression(const char *expression, int numbers[], char operators[], int *numCount, int *opCount) {
    int i = 0;
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
            numbers[(*numCount)++] = number;
            expectNumber = 0;
        } 
        else if (expression[i] == '+' || expression[i] == '-' || 
                 expression[i] == '*' || expression[i] == '/') {
            if (expectNumber) {
                printf("Error: Invalid operator placement near '%c'\n", expression[i]);
                exit(0);
            }
            operators[(*opCount)++] = expression[i];
            i++;
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

int evaluateExpression(int numbers[], char operators[], int numCount, int opCount) {
    for (int i = 0; i < opCount; i++) {
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

            for (int j = i + 1; j < numCount - 1; j++){
                numbers[j] = numbers[j + 1];
            }
            for (int j = i; j < opCount - 1; j++){

                operators[j] = operators[j + 1];
            }
            numCount--;
            opCount--;
            i--;
        }
    }
    int result = numbers[0];
    for (int i = 0; i < opCount; i++) {
        if (operators[i] == '+'){
            result += numbers[i + 1];
        }
        else{
            result -= numbers[i + 1];
        }
    }

    return result;
}
