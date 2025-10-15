#include <stdio.h>
#include <stdlib.h>

int main()
{
    int num, *arr, i;
    scanf("%d", &num);
    arr = (int*) malloc(num * sizeof(int));
    for(i = 0; i < num; i++) {
        scanf("%d", arr + i);
    }


    /* Write the logic to reverse the array. */
    
    int right =num-1;
    int left =0;
    while(left<right){
        int temp = *(arr+left);
        *(arr+left) = *(arr+right);
        *(arr+right) = temp;
        
        left++;
        right--;
    }

    for(i = 0; i < num; i++)
        printf("%d ", *(arr + i));
    return 0;
}
