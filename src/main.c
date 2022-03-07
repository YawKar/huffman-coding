#include <stdio.h>
#include <string.h>
#include <stdlib.h>

typedef unsigned char uChar;

typedef struct Node {
    uChar isSymbol;
    uChar symbol;
    long frequency;
    struct Node *left, *right;
} NODE;

typedef struct NodeList {
    NODE** list;
    int size;
    int capacity;
} NODELIST;

NODE* newNode(uChar isSymbol, uChar symbol, long frequency, NODE* left, NODE* right) {
    NODE* node = (NODE*)malloc(sizeof(NODE));
    node->isSymbol = isSymbol;
    node->symbol = symbol;
    node->frequency = frequency;
    node->left = left;
    node->right = right;
    return node;
}

void ensureNodeListCapacity(NODELIST* nodeList, int capacity) {
    if (nodeList->capacity >= capacity)
        return;
    NODE** newList = (NODE**)malloc(sizeof(NODE*) * capacity);
    for (int i = 0; i < nodeList->size; ++i)
        newList[i] = nodeList->list[i];
    if (nodeList->list)
        free(nodeList->list);
    nodeList->list = newList;
}

NODELIST* newNodeList(int originCapacity) {
    NODELIST* nodeList = (NODELIST*)malloc(sizeof(NODELIST));
    nodeList->size = 0;
    nodeList->capacity = 0;
    nodeList->list = NULL;
    if (originCapacity) {
        ensureNodeListCapacity(nodeList, originCapacity);
    }
    return nodeList;
}

void add2NodeList(NODELIST* nodeList, NODE* node) {
    if (nodeList->size == nodeList->capacity) {
        ensureNodeListCapacity(nodeList, nodeList->capacity * 2 + 1);
    }
    nodeList->list[nodeList->size++] = node;
}

void printNodeList(NODELIST* nodeList) {
    for (int i = 0; i < nodeList->size; ++i) {
        printf("%d ", nodeList->list[i]->frequency);
    }
    printf("\n");
}

int nodeComparator(const void* a, const void * b) {
    const NODE* first = *(const NODE**)a;
    const NODE* second = *(const NODE**)b;
    return first->frequency - second->frequency;
}

void encode(char* inputFilename, char* outputFilename) {
    FILE* inputFile = fopen(inputFilename, "rb");
    if (!inputFile) {
        printf("Cannot open file: %s", inputFilename);
        return;
    }
    fseek(inputFile, 0L, SEEK_END);
    int inputFileLength = ftell(inputFile);
    fseek(inputFile, 0L, SEEK_SET);
    long *frequency = (long*)calloc(256, sizeof(long));
    for (int i = 0; i < inputFileLength; ++i) {
        uChar cur = fgetc(inputFile);
        ++frequency[cur];
    }
    int existCharsNumber = 0;
    for (int i = 0; i < 256; ++i)
        existCharsNumber += frequency[i] != 0L;
    NODELIST* freeNodes = newNodeList(existCharsNumber);
    for (int i = 0; i < 256; ++i) {
        if (frequency[i]) {
            add2NodeList(freeNodes, newNode(1, i, frequency[i], NULL, NULL));
        }
    }
    while (freeNodes->size > 1) {
        qsort(freeNodes->list, freeNodes->size, sizeof(NODE*), nodeComparator);
        NODE* combined = newNode(0,
                                 0,
                                 freeNodes->list[freeNodes->size - 1]->frequency +
                                 freeNodes->list[freeNodes->size - 2]->frequency,
                                 freeNodes->list[freeNodes->size - 1],
                                 freeNodes->list[freeNodes->size - 2]);
        freeNodes->size -= 2;
        add2NodeList(freeNodes, combined);
    }

}

void decode(char* inputFilename, char* outputFilename) {

}

int main() {
    char mode[100] = {0};
    char inputFilename[100] = {0};
    char outputFilename[100] = {0};
    printf("Enter mode: ");
    scanf("%s", mode);
    printf("Enter input file: ");
    scanf("%s", inputFilename);
    printf("Enter output file: ");
    scanf("%s", outputFilename);
    if (strcmp(mode, "encode") == 0) {
        encode(inputFilename, outputFilename);
    } else if (strcmp(mode, "decode") == 0) {
        decode(inputFilename, outputFilename);
    } else {
        printf("Unknown mode: %s", mode);
    }
}