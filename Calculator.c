#include <stdio.h>
#include <ctype.h>

#define MAXIMUM_LENGTH 100

void checkExpression(const char *expression, int numbers[], char operators[], int *numberCount, int *operatorCount);

int calculateExpression(int numbers[], char operators[], int numberCount, int operatorCount);

int main()
{

    char expression[MAXIMUM_LENGTH];

    printf("enter expression to be evaluated:");

    fgets(expression, MAXIMUM_LENGTH, stdin);

    int numbers[MAXIMUM_LENGTH];
    char operators[MAXIMUM_LENGTH];
    int numberCount = 0, operatorCount = 0;

    checkExpression(expression, numbers, operators, &numberCount, &operatorCount);
    int result = calculateExpression(numbers, operators, numberCount, operatorCount);

    printf("The resiult is : %d", result);
    return 0;
}

void checkExpression(const char *expression, int numbers[], char operators[], int *numberCount, int *operatorCount)
{
    int index = 0;
    int invalidOperator = 1;

    while (expression[index])
    {
        if (isspace(expression[index]))
        {
            index++;
            continue;
        }

        if (isdigit(expression[index]))
        {
            int number = 0;
            while (isdigit(expression[index]))
            {
               number = number * 10 + (expression[index] - '0');
                index++;
            }
            numbers[(*numberCount)++] = number;
            invalidOperator = 0;
        }
        else if (expression[index] == '+' || expression[index] == '-' || expression[index] == '*' || expression[index] == '/')
        {
            if (invalidOperator == 1)
            {
                printf("invalid operator placement! please try again");
                return;
            }
            operators[(*operatorCount)++] = expression[index];
            index++;
        }
        else if (expression[index] == '\n')
        {
            break;
        }
        else{
            printf("Invalid character '%c' \n", expression[index]);
            return;
        }
    }
    if(invalidOperator==1){
        printf("expresssion can't end with operator");
        return;
    }
}

int calculateExpression(int numbers[], char operators[], int numberCount, int operatorCount)
{
    for( int index = 0; index<operatorCount; index++){
        if(operators[index] == '*'|| operators[index]=='/'){
            if(operators[index] == '*'){
                numbers[index]*=numbers[index+1];
            } else{
                if(numbers[index+1]==0){
                    printf("division by zero!");
                    return -1;
                }
                numbers[index]/=numbers[index+1];
            }
            for(int updateIndex = index+1;updateIndex<numberCount-1;updateIndex++){
                numbers[updateIndex] = numbers[updateIndex+1];
            }
            for(int updateIndex = index;updateIndex<operatorCount;updateIndex++){
                operators[updateIndex] = operators[updateIndex+1];
            }
            numberCount--;
            operatorCount--;
            index--;
        }

    }
    int result = numbers[0];
    for (int index = 0; index < operatorCount; index++) {
        if (operators[index] == '+') {
            result += numbers[index + 1];
        } else {
            result -= numbers[index + 1];
        }
    }

    return result;
}

