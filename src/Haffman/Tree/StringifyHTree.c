#include "StringifyHTree.h"
#include "../../SharedTypes.h"
#include <string.h>
#include <stdlib.h>

#include <stdio.h>

void stringifyHTree(Node* HTreeRoot, char** result, uInt* curCapacity) {
    if (HTreeRoot->isLeaf) {
        uInt resultLen = strlen(*result);
        if (resultLen == *curCapacity - 1) {
            // Double the string
            char* newResult = (char*)calloc(*curCapacity * 2 + 1, sizeof(char));
            for (int i = 0; i < resultLen; ++i)
                newResult[i] = (*result)[i];
            free(*result);
            *result = newResult;
            *curCapacity = *curCapacity * 2 + 1;
        }
        (*result)[resultLen] = HTreeRoot->byte;
        return;
    }
    // Go to the left branch
    uInt resultLen = strlen(*result);
    if (resultLen == *curCapacity - 1) {
        // Double the string
        char* newResult = (char*)calloc(*curCapacity * 2 + 1, sizeof(char));
        for (int i = 0; i < resultLen; ++i)
            newResult[i] = (*result)[i];
        free(*result);
        *result = newResult;
        *curCapacity = *curCapacity * 2 + 1;
    }
    strcat(*result, "0");
    stringifyHTree(HTreeRoot->left, result, curCapacity);

    // Go to the right branch
    resultLen = strlen(*result);
    if (resultLen == *curCapacity - 1) {
        // Double the string
        char* newResult = (char*)calloc(*curCapacity * 2 + 1, sizeof(char));
        for (int i = 0; i < resultLen; ++i)
            newResult[i] = (*result)[i];
        free(*result);
        *result = newResult;
        *curCapacity = *curCapacity * 2 + 1;
    }
    strcat(*result, "1");
    stringifyHTree(HTreeRoot->right, result, curCapacity);
}