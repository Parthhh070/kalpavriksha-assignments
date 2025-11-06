#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct product
{
    int productId;
    char productName[50];
    float productPrice;
    int productQuantity;
};

struct product *addNewProduct(struct product *numberOfProducts, int *size)
{
    *size = *size + 1;
    numberOfProducts = realloc(numberOfProducts, *size * sizeof(struct product));

    if (numberOfProducts == NULL)
    {
        printf("adding new product failed. pleas try again\n");
    }

    printf("enter the details for new product: \n");

    printf("Product ID:\n ");
    scanf("%d", &numberOfProducts[*size - 1].productId);

    while (getchar() != '\n')
        ;

    printf("Product Name: \n");
    scanf("[^'\n']", numberOfProducts[*size - 1].productName);

    printf("Product Price: \n");
    scanf("%f", &numberOfProducts[*size - 1].productPrice);

    printf("Product quantity: \n");
    scanf("%d", &numberOfProducts[*size - 1].productQuantity);

    printf("new product added successfully!! \n");

    return numberOfProducts;
}

void viewAllProducts(struct product *numberOfProducts, int size)
{
    printf("here is the full product list.\n");

    for (int index = 0; index < size; index++)
    {
        printf("ProductID: %d | Productname: %s | ProductPrice: %f | ProductQuantity: %d \n", numberOfProducts[index].productId,
               numberOfProducts[index].productName,
               numberOfProducts[index].productPrice,
               numberOfProducts[index].productQuantity);
    }
}

void updateQuantity(struct product *numberOfProducts, int size)
{
    int id;
    printf("Enter Product ID to update quantity: \n");
    scanf("%d", &id);
    int found = 0;
    for (int index = 0; index < size; index++)
    {
        if (numberOfProducts[index].productId == id)
        {
            int newQuantity;
            printf("Enter new Quantity: \n");
            scanf("%d", &newQuantity);

            numberOfProducts[index].productQuantity = newQuantity;
            printf("product quantity updated for id %d\n", id);
            found = 1;
            break;
        }
        else
        {
            continue;
        }
    }

    if (found == 0)
    {
        printf("product with id %d is not found\n", id);
    }
}

void searchProductByID(struct product *numberOfProducts, int size)
{
    int id;
    printf("Enter Product ID to search: \n");
    scanf("%d", &id);

    int found = 0;
    for (int index = 0; index < size; index++)
    {
        if (numberOfProducts[index].productId == id)
        {

            printf("ProductID: %d | Productname: %s | ProductPrice: %f | ProductQuantity: %d \n", numberOfProducts[index].productId,
               numberOfProducts[index].productName,
               numberOfProducts[index].productPrice,
               numberOfProducts[index].productQuantity);
            found = 1;
            break;
        }
        else
        {
            continue;
        }
    }

    if (found == 0)
    {
        printf("product with id %d is not found", id);
    }
}

int containPartialName(char *productName, char *searchedQuery)
{
    int productNameLength = strlen(productName);
    int queryLength = strlen(searchedQuery);

    for (int index = 0; index <= productNameLength - queryLength; index++)
    {
        int indexOfQuery;
        for (indexOfQuery = 0; indexOfQuery < queryLength; indexOfQuery++)
        {
            if (productName[index + indexOfQuery] != searchedQuery[indexOfQuery])
            {
                break;
            }
        }
        if (indexOfQuery == queryLength)
        {
            return 1;
        }
    }
    return 0;
}

void searchByName(struct product *numberOfProducts, int size)
{
    char Name[50];
    while (getchar() != '\n')
        ;
    printf("Enter product name to search: \n");
    scanf(" %[^\n]", Name);

    int found = 0;
    for (int index = 0; index < size; index++)
    {
        if (containPartialName(numberOfProducts[index].productName, Name) == 1)
        {
            printf("ProductID: %d | Productname: %s | ProductPrice: %f | ProductQuantity: %d \n", numberOfProducts[index].productId,
               numberOfProducts[index].productName,
               numberOfProducts[index].productPrice,
               numberOfProducts[index].productQuantity);
            found = 1;
        }
    }
    if (found == 0)
    {
        printf("product with %s is not found\n", Name);
    }
}

void searchByPriceRange(struct product *numberOfProducts, int size)
{
    float minimumprice;
    float maximumprice;

    printf("enter the minimum price range: \n");
    scanf("%f", &minimumprice);
    printf("enter the maximum price range: \n");
    scanf("%f", &maximumprice);

    int found = 0;
    for (int index = 0; index < size; index++)
    {
        if (numberOfProducts[index].productPrice >= minimumprice && numberOfProducts[index].productPrice <= maximumprice)
        {
            printf("ProductID: %d | Productname: %s | ProductPrice: %f | ProductQuantity: %d \n", numberOfProducts[index].productId,
               numberOfProducts[index].productName,
               numberOfProducts[index].productPrice,
               numberOfProducts[index].productQuantity);
            found = 1;
        }
    }

    if (found == 0)
    {
        printf("no product is found within this price range\n");
    }
}

void deleteProduct(struct product *numberOfProducts, int *size)
{
    int id;
    printf("Enter Product ID to delete: \n");
    scanf("%d", &id);

    int foundIndex = -1;
    for (int index = 0; index < *size; index++)
    {
        if (numberOfProducts[index].productId == id)
        {
            foundIndex = index;
            break;
        }
    }
    if (foundIndex == -1)
    {
        printf("No product foun with this id.\n");
        return;
    }

    for (int index = foundIndex; index < *size - 1; index++)
    {
        numberOfProducts[index].productId = numberOfProducts[index + 1].productId;
        strcpy(numberOfProducts[index].productName, numberOfProducts[index + 1].productName);
        numberOfProducts[index].productPrice = numberOfProducts[index + 1].productPrice;
        numberOfProducts[index].productQuantity = numberOfProducts[index + 1].productQuantity;
    }
    printf("product with id %d is deleted \n", id);
    *size = *size - 1;
}

void addInitialProducts(struct product *numberOfProducts, int size)
{
    printf("enter the details for each products: \n");
    printf("a. Product ID\n");
    printf("b. Product Name\n");
    printf("c. Product Price\n");
    printf("d. Product Quantity\n");

    for (int index = 0; index < size; index++)
    {
        printf("enter the details for product %d: \n", index + 1);

        printf("Product ID:\n ");
        scanf("%d", &numberOfProducts[index].productId);

        while ((getchar()) != '\n')
            ;

        printf("Product Name: \n");
        scanf("%[^\n]", numberOfProducts[index].productName);

        printf("Product Price: \n");
        scanf("%f", &numberOfProducts[index].productPrice);

        printf("Product quantity: \n");
        scanf("%d", &numberOfProducts[index].productQuantity);

        while ((getchar()) != '\n')
            ;
    }
}
int main()
{

    int size;
    printf("enter the initial number of products; \n ");
    scanf("%d", &size);

    struct product *numberOfProducts = calloc(size, sizeof(struct product));
    if (numberOfProducts == NULL)
    {
        printf("error while allocating the memory. please try again");
        exit(1);
    }

    addInitialProducts(numberOfProducts, size);

    int true = 1;
    while (true == 1)
    {
        printf("====== Inventory Menu ======\n");
        printf("1. Add New Product\n");
        printf("2.  View All Products \n");
        printf("3. Update Quantity\n");
        printf("4. Search Product by ID\n ");
        printf("5. Search Product by Name\n ");
        printf("6. Search Product by Price Range\n ");
        printf("7. Delete Product\n");
        printf("8. Exit\n");

        int choice;
        printf("enter your choice: ");
        scanf("%d", &choice);
        switch (choice)
        {
        case 1:
            numberOfProducts = addNewProduct(numberOfProducts, &size);
            break;
        case 2:
            viewAllProducts(numberOfProducts, size);
            break;
        case 3:
            updateQuantity(numberOfProducts, size);
            break;
        case 4:
            searchProductByID(numberOfProducts, size);
            break;
        case 5:
            searchByName(numberOfProducts, size);
            break;
        case 6:
            searchByPriceRange(numberOfProducts, size);
            break;
        case 7:
            deleteProduct(numberOfProducts, &size);
            break;
        case 8:
            true = 0;
            free(numberOfProducts);
            numberOfProducts = NULL;
            printf("Memory released successfully. Exiting program..");
            break;
        default:
            printf("please enter a valid choice. \n");
            break;
        }
    }

    return 0;
}
