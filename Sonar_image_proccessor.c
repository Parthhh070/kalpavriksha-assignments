#include <stdio.h>
#include <stdlib.h>



void generateMatrix(int **matrix, int size){
    if (matrix == NULL || size <= 0) return;


    for(int row=0;row<size;row++){
        for(int column=0;column<size;column++){
            *(*(matrix+row)+column)=rand()%256;
        }
    }

}

void displayMatrix(int **matrix, int size){

    if (matrix == NULL || size <= 0) return;

    for(int row=0;row<size;row++){
        for(int column=0;column<size;column++){
            printf("%d ",*(*(matrix+row)+column));
        }
        printf("\n");
    }
    printf("\n");
}


void rotateClockwise(int **matrix, int size){

    if (matrix == NULL || size <= 0) return;


    for(int row=0;row<size;row++){
        for(int column=row+1;column<size;column++){
           int temporaryVariable = *(*(matrix+row)+column) ;
           *(*(matrix+row)+column) = *(*(matrix+column)+row) ;
           *(*(matrix+column)+row) = temporaryVariable;
        }
    }

    for(int row=0;row<size;row++){
        int start=0;
        int end = size-1;

        while(start<end){
            int temporaryVariable = *(*(matrix+row)+start);
            *(*(matrix+row)+start) = *(*(matrix+row)+end);
            *(*(matrix+row)+end) = temporaryVariable;
          
            start++;
            end--;
        }
    }
}


void applyfilter(int** matrix, int size){

    if (matrix == NULL || size <= 0) return;

    int *temporaryRow = malloc(sizeof(int)*size);
    int *previousRow = malloc(sizeof(int)*size);


    for(int row=0;row<size;row++){
        int *currentRow = *(matrix+row);
        int *nextRow = (row < size - 1) ? *(matrix + row + 1) : NULL;

        for(int index=0;index<size;index++){
            *(temporaryRow+index) = *(currentRow+index);
        }

        for(int column=0;column<size;column++){
            int sum=0;
            int count=0;

            for(int directionI=-1;directionI<=1;directionI++){
                for(int directionJ=-1;directionJ<=1;directionJ++){
                    int numberAtI= directionI+row;
                    int numberAtJ = directionJ+column;


                    if(numberAtI>=0 && numberAtI<size && numberAtJ >=0 && numberAtJ <size){
                        int value;
                        if(numberAtI == row-1){
                            value = *(previousRow+numberAtJ);
                        } else if (numberAtI==row)
                        {
                            value = *(currentRow+numberAtJ);                       
                        } else{
                            value = *(nextRow+numberAtJ);
                         }  
                          sum+=value;  
                          count++;
                    }
                    
                }
                
            }
            *(currentRow+column) = sum/count;
        }

         for (int index = 0; index < size; ++index)
        {
            *(previousRow + index) = *(temporaryRow + index);
        }
    }

    free(temporaryRow);
    free(previousRow);
}
int main(){
    int size;
    printf("Enter matrix size (2-10): ");
    scanf("%d", &size);


    int **matrix = malloc(sizeof(int*) * size);

    
    for(int i=0;i<size;i++){
        *(matrix+i) = malloc(sizeof(int)* size);
    }

    generateMatrix(matrix,size);
    printf("Original Randomly Generated Matrix:\n");
    displayMatrix(matrix,size);

    rotateClockwise(matrix,size);
    printf("Matrix after 90 degree Clockwise Rotation: \n");
    displayMatrix(matrix,size);

    applyfilter( matrix,  size);
    printf("Matrix after Applying 3 x 3 Smoothing Filter: \n");
    displayMatrix(matrix,size);

    for(int i=0;i<size;i++){
        free(*(matrix+i));
    }
    free(matrix);
    return 0;
}