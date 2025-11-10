#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAXIMUM_BLOCKS_ALLOWED 1024
#define SINGLE_BLOCK_SIZE 512
#define MAXIMUM_BLOCKS_PER_FILE 10
typedef struct FreeBlock
{
    int index;
    struct FreeBlock *prev;
    struct FreeBlock *next;

} FreeBlock;

typedef struct FileNode
{
    char name[50];
    int isDirectory;
    struct FileNode *parent;
    struct FileNode *next;
    struct FileNode *child;
    int blockPointers[MAXIMUM_BLOCKS_PER_FILE];
    int size;
} FileNode;

FileNode *root, *cwd;
FreeBlock *freeListHead = NULL, *freeListTail = NULL;
char virtualDisk[MAXIMUM_BLOCKS_ALLOWED][SINGLE_BLOCK_SIZE];
int usedBlocks = 0;

int allocateBlock()
{
    if (freeListHead == NULL)
    {

        return -1;
    }
    FreeBlock *block = freeListHead;
    int index = block->index;
    freeListHead = block->next;
    if (freeListHead)
    {
        freeListHead->prev = NULL;
    }
    else
    {
        freeListTail = NULL;
    }

    free(block);
    usedBlocks++;
    return index;
}

void freeBlock(int index)
{
    FreeBlock *block = (FreeBlock *)malloc(sizeof(FreeBlock));

    if (!block)
    {
        printf("Memory allocation failed for free block.\n");
        exit(1);
    }
    block->index = index;
    block->next = NULL;
    block->prev = freeListTail;

    if (freeListTail)
    {
        freeListTail->next = block;
    }
    else
    {
        freeListHead = block;
    }
    freeListTail = block;
    usedBlocks--;
}

void initialize()
{
    root = (FileNode *)malloc(sizeof(FileNode));

    if (!root)
    {
        printf("Memory allocation failed for free block.\n");
        exit(1);
    }
    strcpy(root->name, "/");
    root->isDirectory = 1;
    root->parent = NULL;
    root->child = NULL;
    root->next = NULL;
    cwd = root;

    // Initialize virtual disk
    for (int i = 0; i < MAXIMUM_BLOCKS_ALLOWED; i++)
        memset(virtualDisk[i], 0, SINGLE_BLOCK_SIZE);

    // Build free block list
    for (int i = 0; i < MAXIMUM_BLOCKS_ALLOWED; i++)
    {
        FreeBlock *block = (FreeBlock *)malloc(sizeof(FreeBlock));

        if (!block)
        {
            printf("Memory allocation failed for free block.\n");
            exit(1);
        }

        block->index = i;

        block->next = NULL;

        block->prev = freeListTail;

        if (freeListTail)
        {

            freeListTail->next = block;
        }
        else
        {
            freeListHead = block;
        }

        freeListTail = block;
    }

    usedBlocks = 0;
    printf("Compact VFS - ready. Type 'exit' to quit.\n");
}

void mkdirCmd(char *name)
{
    FileNode *newDir = (FileNode *)malloc(sizeof(FileNode));
      if (!newDir) {
        printf("Memory allocation failed for free block.\n");
        exit(1);
    }
    strcpy(newDir->name, name);
    newDir->isDirectory = 1;
    newDir->parent = cwd;
    newDir->child = NULL;
    newDir->next = NULL;
    for (int i = 0; i < MAXIMUM_BLOCKS_PER_FILE; i++)
        newDir->blockPointers[i] = -1;

    if (cwd->child == NULL)
    {
        cwd->child = newDir;
        newDir->next = newDir;
    }
    else
    {
        FileNode *temp = cwd->child;
        while (temp->next != cwd->child)
            temp = temp->next;
        temp->next = newDir;
        newDir->next = cwd->child;
    }

    printf("Directory '%s' created successfully.\n", name);
}

void createcmd(char *name)
{
    FileNode *newFile = (FileNode *)malloc(sizeof(FileNode));
      if (!newFile) {
        printf("Memory allocation failed for free block.\n");
        exit(1);
    }
    strcpy(newFile->name, name);
    newFile->isDirectory = 0;
    newFile->parent = cwd;
    newFile->child = NULL;
    newFile->next = NULL;
    newFile->size = 0;

    for (int i = 0; i < MAXIMUM_BLOCKS_PER_FILE; i++)
        newFile->blockPointers[i] = -1;

    if (cwd->child == NULL)
    {

        cwd->child = newFile;

        newFile->next = newFile;
    }
    else
    {

        FileNode *temp = cwd->child;
        while (temp->next != cwd->child)
        {
            temp = temp->next;
        }
        temp->next = newFile;
        newFile->next = cwd->child;
    }

    printf("File '%s' created successfully.\n", name);
}

void writeCmd(char *name, char *data)
{
    FileNode *temp = cwd->child;
    if (!temp)
    {
        printf("No such file.\n");
        return;
    }

    do
    {
        if (!temp->isDirectory && strcmp(temp->name, name) == 0)
        {
            int dataLen = strlen(data);
            temp->size = dataLen;

            // Free old blocks
            for (int i = 0; i < MAXIMUM_BLOCKS_PER_FILE; i++)
            {
                if (temp->blockPointers[i] != -1)
                {
                    freeBlock(temp->blockPointers[i]);
                    temp->blockPointers[i] = -1;
                }
            }

            int blocksNeeded = (dataLen + SINGLE_BLOCK_SIZE - 1) / SINGLE_BLOCK_SIZE;
            int dataIndex = 0;

            for (int i = 0; i < blocksNeeded && i < MAXIMUM_BLOCKS_PER_FILE; i++)
            {
                int blockIndex = allocateBlock();
                if (blockIndex == -1)
                {
                    printf("Disk full! Data partially written.\n");
                    return;
                }

                temp->blockPointers[i] = blockIndex;
                int bytesToCopy = (dataLen - dataIndex > SINGLE_BLOCK_SIZE) ? SINGLE_BLOCK_SIZE : dataLen - dataIndex;
                memcpy(virtualDisk[blockIndex], data + dataIndex, bytesToCopy);
                dataIndex += bytesToCopy;
            }

            printf("Data written successfully (size -> %d bytes).\n", temp->size);
            return;
        }
        temp = temp->next;
    } while (temp != cwd->child);

    printf("File not found.\n");
}

void readCmd(char *name)
{
    FileNode *temp = cwd->child;
    if (!temp)
    {
        printf("No such file.\n");
        return;
    }

    do
    {
        if (!temp->isDirectory && strcmp(temp->name, name) == 0)
        {
            for (int i = 0; i < MAXIMUM_BLOCKS_PER_FILE && temp->blockPointers[i] != -1; i++)
            {
                printf("%s", virtualDisk[temp->blockPointers[i]]);
            }
            printf("\n");
            return;
        }
        temp = temp->next;
    } while (temp != cwd->child);

    printf("File not found.\n");
}

void deleteCmd(char *name)
{
    if (cwd->child == NULL)
    {
        printf("No such file.\n");
        return;
    }

    FileNode *temp = cwd->child;
    do
    {
        if (!temp->isDirectory && strcmp(temp->name, name) == 0)
        {
            for (int i = 0; i < MAXIMUM_BLOCKS_PER_FILE; i++)
            {
                if (temp->blockPointers[i] != -1)
                    freeBlock(temp->blockPointers[i]);
            }

            // Remove from circular list
            if (temp == cwd->child && temp->next == cwd->child)
                cwd->child = NULL;
            else
            {
                FileNode *end = cwd->child;
                while (end->next != cwd->child)
                    end = end->next;
                if (temp == cwd->child)
                    cwd->child = temp->next;
                end->next = temp->next;
            }

            free(temp);

            printf("File deleted successfully \n");
            return;
        }
        temp = temp->next;
    } while (temp != cwd->child);

    printf("File not foundd.\n");
}

void rmdirCmd(char *name)
{
    if (!cwd->child)
    {
        printf("No such directory.\n");
        return;
    }

    FileNode *temp = cwd->child;
    do
    {
        if (temp->isDirectory && strcmp(temp->name, name) == 0)
        {
            if (temp->child != NULL)
            {
                printf("Directory is not empty.\n");
                return;
            }

            if (temp == cwd->child && temp->next == cwd->child)
                cwd->child = NULL;
            else
            {
                FileNode *end = cwd->child;
                while (end->next != cwd->child)
                    end = end->next;
                if (temp == cwd->child)
                    cwd->child = temp->next;
                end->next = temp->next;
            }

            free(temp);
            printf("Directory removed successfully.\n");
            return;
        }
        temp = temp->next;
    } while (temp != cwd->child);

    printf("Directory not found.\n");
}

void cdCmd(char *name)
{
    if (strcmp(name, "..") == 0)
    {
        if (cwd->parent)
            cwd = cwd->parent;
        else
            printf("Already at root.\n");
        return;
    }

    FileNode *temp = cwd->child;
    if (!temp)
    {
        printf("No such directory.\n");
        return;
    }

    do
    {
        if (temp->isDirectory && strcmp(temp->name, name) == 0)
        {
            cwd = temp;
            printf("Moved to /%s\n", cwd->name);
            return;
        }
        temp = temp->next;
    } while (temp != cwd->child);

    printf("Directory not found.\n");
}

void lsCmd()
{
    if (!cwd->child)
    {
        printf("(empty)\n");
        return;
    }

    FileNode *temp = cwd->child;
    do
    {
        printf("%s%s\n", temp->name, temp->isDirectory ? "/" : "");
        temp = temp->next;
    } while (temp != cwd->child);
}

void pwdCmd()
{
    char path[100] = "";
    FileNode *temp = cwd;
    while (temp)
    {
        char tempPath[100];
        sprintf(tempPath, "%s%s", strcmp(temp->name, "/") == 0 ? "" : "/", temp->name);
        strcat(tempPath, path);
        strcpy(path, tempPath);
        temp = temp->parent;
    }
    printf("%s\n", strlen(path) ? path : "/");
}

void dfCmd()
{
    int freeBlocks = MAXIMUM_BLOCKS_ALLOWED - usedBlocks;
    float usage = ((float)usedBlocks / MAXIMUM_BLOCKS_ALLOWED) * 100;
    printf("Total Blocks: %d\n", MAXIMUM_BLOCKS_ALLOWED);

    printf("Used Blocks: %d\n", usedBlocks);

    printf("Free Blocks: %d\n", freeBlocks);

    printf("Disk Usge: %.2f%%\n", usage);
}

void executecmd()
{
    char command[50], fileOrdirName[50], fileContent[200];

    while (1)
    {
        printf("%s > ", cwd->name);
        scanf("%s", command);

        if (strcmp(command, "mkdir") == 0)
        {
            scanf("%s", fileOrdirName);
            mkdirCmd(fileOrdirName);
        }
        else if (strcmp(command, "create") == 0)
        {
            scanf("%s", fileOrdirName);
            createcmd(fileOrdirName);
        }
        else if (strcmp(command, "write") == 0)
        {
            scanf("%s", fileOrdirName);
            getchar();
            fgets(fileContent, sizeof(fileContent), stdin);
            fileContent[strcspn(fileContent, "\n")] = 0;
            writeCmd(fileOrdirName, fileContent);
        }
        else if (strcmp(command, "read") == 0)
        {
            scanf("%s", fileOrdirName);
            readCmd(fileOrdirName);
        }
        else if (strcmp(command, "delete") == 0)
        {
            scanf("%s", fileOrdirName);
            deleteCmd(fileOrdirName);
        }
        else if (strcmp(command, "rmdir") == 0)
        {
            scanf("%s", fileOrdirName);
            rmdirCmd(fileOrdirName);
        }
        else if (strcmp(command, "cd") == 0)
        {
            scanf("%s", fileOrdirName);
            cdCmd(fileOrdirName);
        }
        else if (strcmp(command, "ls") == 0)
        {
            lsCmd();
        }
        else if (strcmp(command, "pwd") == 0)
        {
            pwdCmd();
        }
        else if (strcmp(command, "df") == 0)
        {
            dfCmd();
        }
        else if (strcmp(command, "exit") == 0)
        {
            printf("Memory released. Exiting program...\n");
            break;
        }
        else
        {
            printf("Please enter a valid command.\n");
        }
    }
}

int main()
{
    initialize();
    executecmd();
    return 0;
}
