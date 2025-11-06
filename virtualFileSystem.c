#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_BLOCKS 1024
#define BLOCK_SIZE 512
#define MAX_BLOCKS_PER_FILE 10

typedef struct FileNode
{
    char Name[50];
    int isDirectory;
    struct FileNode *parent;
    struct FileNode *child;
    struct FileNode *next;
    int blockPointers[MAX_BLOCKS_PER_FILE];
    int size;
    char content[MAX_BLOCKS_PER_FILE * BLOCK_SIZE];

} FileNode;

FileNode *root, *cwd;
int disk[MAX_BLOCKS];
int usedBlocks = 0;

int allocateBlock()
{
    for (int i = 0; i < MAX_BLOCKS; i++)
    {
        if (disk[i] == 0)
        {
            disk[i] = 1;
            usedBlocks++;
            return i;
        }
    }
    return -1;
}

void freeblock(int index)
{
    if (index >= 0 && index < MAX_BLOCKS && disk[index] == 1)
    {
        disk[index] = 0;
        usedBlocks--;
    }
}

void initialize()
{
    root = (FileNode *)malloc(sizeof(FileNode));
    strcpy(root->Name, "/");
    root->isDirectory = 1;
    root->parent = NULL;
    root->child = NULL;
    root->next = NULL;
    cwd = root;
    memset(disk, 0, sizeof(disk));
    printf("Compact VFS-ready. Type 'exit' to quitt.\n");
}

void mkdir(char *name)
{
    FileNode *newDir = (FileNode *)malloc(sizeof(FileNode));
    strcpy(newDir->Name, name);
    newDir->isDirectory = 1;
    newDir->parent = cwd;
    newDir->child = NULL;
    newDir->next = NULL;

    if (cwd->child == NULL)
    {
        cwd->child = newDir;
        newDir->next = newDir;
    }
    else
    {
        FileNode *temp = cwd->child;
        while (temp->next != cwd->child)
        {
            temp = temp->next;
        }
        temp->next = newDir;
        newDir->next = cwd->child;
    }

    printf("directory '%s' is   created successfully \n", name);
}

void read(char *name)
{
    FileNode *temp = cwd->child;
    if (temp == NULL)
    {
        printf("No such file.\n");
        return;
    }

    do
    {
        if (!temp->isDirectory && strcmp(temp->Name, name) == 0)
        {
            printf("%s\n", temp->content);
            return;
        }
        temp = temp->next;
    } while (temp != cwd->child);

    printf("File not found.\n");
}

void create(char *name)
{
    FileNode *newFile = (FileNode *)malloc(sizeof(FileNode));
    strcpy(newFile->Name, name);
    newFile->isDirectory = 0;
    newFile->parent = cwd;
    newFile->child = NULL;
    newFile->next = NULL;
    newFile->size = 0;

    memset(newFile->blockPointers, -1, sizeof(newFile->blockPointers));

    if (cwd->child == NULL)
    {
        cwd->child = newFile;
        newFile->next = newFile;
    }
    else
    {
        FileNode *temp = cwd->child;
        while (temp->next != cwd->child)
            temp = temp->next;
        temp->next = newFile;
        newFile->next = cwd->child;
    }

    printf("File '%s' created successfully.\n", name);
}

void write(char *name, char *data)
{
    FileNode *temp = cwd->child;
    if (temp == NULL)
    {
        printf("No such file.\n");
        return;
    }

    do
    {
        if (!temp->isDirectory && strcmp(temp->Name, name) == 0)
        {
            strcpy(temp->content, data);
            temp->size = strlen(data);

            int block = allocateBlock();
            if (block == -1)
            {
                printf("free blocks not available.\n");
                return;
            }
            temp->blockPointers[0] = block;

            printf("Data written successfully (size=%d bytes).\n", temp->size);
            return;
        }
        temp = temp->next;
    } while (temp != cwd->child);

    printf("File not found.\n");
}

void delete(char *name)
{

    if (cwd->child == NULL)
    {
        printf("No such file.\n");
        return;
    }

    FileNode *temp = cwd->child, *prev = NULL;

    do
    {
        if (!temp->isDirectory && strcmp(temp->Name, name) == 0)
        {
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

            for (int i = 0; i < MAX_BLOCKS_PER_FILE; i++)
                if (temp->blockPointers[i] != -1)
                    freeblock(temp->blockPointers[i]);

            free(temp);
            printf("File deleted successfully.\n");
            return;
        }
        prev = temp;
        temp = temp->next;
    } while (temp != cwd->child);

    printf("File not found.\n");
}

void rmdir(char *name)
{
    if (cwd->child == NULL)
    {
        printf("No such directory.\n");
        return;
    }

    FileNode *temp = cwd->child, *prev = NULL;
    do
    {
        if (temp->isDirectory && strcmp(temp->Name, name) == 0)
        {
            if (temp->child != NULL)
            {
                printf("Directory is not empty.\n");
                return;
            }

            FileNode *end = cwd->child;
            while (end->next != cwd->child)
                end = end->next;

            if (temp == cwd->child)
                cwd->child = temp->next;
            end->next = temp->next;

            free(temp);
            printf("Directory removed successfuflly.\n");
            return;
        }

        prev = temp;
        temp = temp->next;
    } while (temp != cwd->child);

    printf("Directory not found\n");
}

void cd(char *name)
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
    if (temp == NULL)
    {
        printf("No such directory.\n");
        return;
    }

    do
    {
        if (temp->isDirectory && strcmp(temp->Name, name) == 0)
        {
            cwd = temp;
            printf("Moveed to /%s\n", cwd->Name);

            return;
        }
        temp = temp->next;
    } while (temp != cwd->child);

    printf("Directory not found.\n");
}

void ls()
{
    if (cwd->child == NULL)
    {
        printf("(empty)\n");
        return;
    }

    FileNode *temp = cwd->child;
    do
    {
        printf("%s%s\n", temp->Name, temp->isDirectory ? "/" : "");
        temp = temp->next;
    } while (temp != cwd->child);
}

void pwd()
{
    char path[100] = "";
    FileNode *temp = cwd;
    while (temp != NULL)
    {
        char tempPath[100];
        sprintf(tempPath, "%s%s", strcmp(temp->Name, "/") == 0 ? "" : "/", temp->Name);

        strcat(tempPath, path);

        strcpy(path, tempPath);
        temp = temp->parent;
    }

    printf("%s\n", strlen(path) ? path : "/");
}

void df()
{
    int freeBlocks = MAX_BLOCKS - usedBlocks;

    float usage = ((float)usedBlocks / MAX_BLOCKS) * 100;

    printf("Total Bloks: %d\n", MAX_BLOCKS);
    printf("Used blocks: %d\n", usedBlocks);
    printf("Free Blocks: %d\n", freeBlocks);
    printf("Disk Usage: %.2f%%\n", usage);
}

int main()
{
    initialize();

    char command[50], arg1[50], arg2[200];

    while (1)
    {
        printf("%s > ", cwd->Name);
        scanf("%s", command);

        if (strcmp(command, "mkdir") == 0)
        {
            scanf("%s", arg1);

            mkdir(arg1);
        }
        else if (strcmp(command, "create") == 0)
        {
            scanf("%s", arg1);

            create(arg1);
        }
        else if (strcmp(command, "write") == 0)
        {
            scanf("%s", arg1);
            getchar();

            fgets(arg2, sizeof(arg2), stdin);
            arg2[strcspn(arg2, "\n")] = 0;
            write(arg1, arg2);
        }
        else if (strcmp(command, "read") == 0)
        {
            scanf("%s", arg1);

            read(arg1);
        }
        else if (strcmp(command, "delete") == 0)
        {
            scanf("%s", arg1);
            delete(arg1);
        }
        else if (strcmp(command, "rmdir") == 0)
        {
            scanf("%s", arg1);
            rmdir(arg1);
        }
        else if (strcmp(command, "cd") == 0)
        {
            scanf("%s", arg1);
            cd(arg1);
        }
        else if (strcmp(command, "ls") == 0)
        {
            ls();
        }
        else if (strcmp(command, "pwd") == 0)
        {
            pwd();
        }
        else if (strcmp(command, "df") == 0)
        {
            df();
        }
        else if (strcmp(command, "exit") == 0)
        {
            printf("Memory released. exiting program\n");
            break;
        }
        else
        {
            printf("Unknown input!\n");
        }
    }
    return 0;
}
