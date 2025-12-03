#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define HASH_SIZE 1009

typedef struct Node
{
    int key;
    char value[100];
    struct Node *previous;
    struct Node *next;
} Node;

typedef struct Map
{
    int key;
    Node *Node;
    struct Map *next;
}Map;


typedef struct LRUCache{
    int capacity;
    int size;
    Node *head;
    Node *tail;
    Map* map[HASH_SIZE];
} LRUCache;


int hash(int key) {
    return key % HASH_SIZE;
}

Node *mapGet(LRUCache *cache, int key){
    int h = hash(key);
    Map *curr = cache->map[h];
    while (curr!=NULL)
    {
        if (curr->key == key){
            return curr->Node;
        } 
        curr = curr->next;
    }
    return NULL;
    
}


void mapPut(LRUCache *cache, int key,Node *Node)
{
     int h = hash(key);
     Map *entry = (Map*) malloc(sizeof(Map));
     entry->key = key;
     entry->Node=Node;
     entry->next = cache->map[h];
     cache->map[h] = entry; 
}

void mapRemove(LRUCache *cache, int key){
    int h = hash(key);
    Map *curr = cache->map[h], *prev =NULL;

    while(curr!=NULL){
        if(curr->key == key){
            if(prev){
                prev->next = curr->next;    
            }else{
                cache->map[h] = curr->next;
            }
            free(curr);
            return;
        }
        prev = curr;
        curr = curr->next;
    }
}


void removeNode(LRUCache* cache, Node* node) {
    if (!node) return;

    if (node->previous!=NULL){
        node->previous->next = node->next;
    } 
    else {
        cache->head = node->next;
    }

    if (node->next!=NULL){
        node->next->previous = node->previous;
    } 
    else {
        cache->tail = node->previous;
    }
}


void insertAtHead(LRUCache* cache, Node* Node) {
    Node->previous = NULL;
    Node->next = cache->head;

    if (cache->head) {
        cache->head->previous = Node;
    }
    cache->head = Node;

    if (cache->tail == NULL) {
        cache->tail = Node;
    }
}


LRUCache *createCache(int capacity){
    LRUCache *cache = (LRUCache*) malloc(sizeof(LRUCache));
    cache->capacity = capacity;
    cache->size = 0;

    cache->head = NULL;

    cache->tail = NULL;


    for (int i = 0; i < HASH_SIZE; i++){
         cache->map[i] = NULL;

    }
       
    return cache;

}


char *get(LRUCache *cache, int key){
    Node *node =mapGet(cache,key);

    if(node==NULL){
        return NULL;
    }
    removeNode(cache,node);
    insertAtHead(cache,node);

    return node->value;
}

void put(LRUCache *cache, int key, char *value){
    Node *existing = mapGet(cache,key);

    if(existing!=NULL){
        strcpy(existing->value, value);

         removeNode(cache, existing);
        insertAtHead(cache, existing);
        return;
    }


    Node* newNode = (Node*)malloc(sizeof(Node));

    newNode->key = key;
    strcpy(newNode->value, value);
    newNode->previous = newNode->next = NULL;


    insertAtHead(cache, newNode);


    mapPut(cache, key, newNode);
    cache->size++;

    if (cache->size > cache->capacity) {
        Node* leastUsed = cache->tail;

        mapRemove(cache, leastUsed->key);
        removeNode(cache, leastUsed);
        free(leastUsed);

        cache->size--;
    }
}


int main(){
    LRUCache *cache = NULL;
    char command[20];

    while (scanf("%s", command) != EOF)
    {
       if (strcmp(command, "createCache") == 0) {
            int cap;
            scanf("%d", &cap);
            cache = createCache(cap);
        }

        else if (strcmp(command, "put") == 0) {
            int key;
            char data[100];
            scanf("%d %s", &key, data);
            put(cache, key, data);
        }

        else if (strcmp(command, "get") == 0) {
            int key;
            scanf("%d", &key);
            char* val = get(cache, key);
            if (val) printf("%s\n", val);
            else printf("null\n");
        }
    }
    
    return 0;
}