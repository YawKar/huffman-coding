#include "ExtractCodes.h"
#include <string.h>
#include <stdlib.h>

void extractCodesFromHTree(Node* HTreeRoot, char** codes, char* curCode) {
    if (HTreeRoot->isLeaf) {
        if (strlen(curCode) == 0) {
            strcat(curCode, "0");
        }
        codes[HTreeRoot->byte] = (char*)malloc(sizeof(char) * (strlen(curCode) + 1));
        strcpy(codes[HTreeRoot->byte], curCode);
        return;
    }
    strcat(curCode, "0");
    extractCodesFromHTree(HTreeRoot->left, codes, curCode);
    curCode[strlen(curCode) - 1] = '1';
    extractCodesFromHTree(HTreeRoot->right, codes, curCode);
    curCode[strlen(curCode) - 1] = '\0';
}