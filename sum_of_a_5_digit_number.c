#include <stdio.h>
#include <string.h>
#include <math.h>
#include <stdlib.h>

int main() {
	
    int number;
    int result=0;
    scanf("%d", &number);
    while(number>0){
        int lastDigit = number%10;
        result = result + lastDigit;
        number /=10;
    }
    printf("%d", result);
    //Complete the code to calculate the sum of the five digits on n.
    return 0;
}
