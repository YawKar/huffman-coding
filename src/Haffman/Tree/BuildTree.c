#include <stdio.h>
#include <stdlib.h>

#include "BuildTree.h"

int nodesComparator(const void *a, const void *b) {
    Node *first = *(Node**)a;
    Node *second = *(Node**)b;
    // First the greatest, last the least
    return second->weight - first->weight;
}

Node* buildTree(const uInt* occurrencesCount) {
    // Count how many bytes exist
    uInt bytesExist = 0;
    for (uInt byte = 0; byte < 256; ++byte)
        bytesExist += occurrencesCount[byte] != 0;

    // Create a node for each of the bytes
    Node** freeNodes = (Node**)malloc(sizeof(Node*) * bytesExist);
    {
        uInt nodeIndex = 0;
        for (uInt byte = 0; byte < 256; ++byte) {
            if (occurrencesCount[byte]) {
                freeNodes[nodeIndex++] = newNode(byte, 1, occurrencesCount[byte], NULL, NULL);
            }
        }
    };

    // Main loop
    uInt nodesLeft = bytesExist;
    while (nodesLeft > 1) {
        qsort(freeNodes, nodesLeft, sizeof(Node*), nodesComparator);
        Node* combined = newNode(
                0,
                0,
                freeNodes[nodesLeft - 1]->weight + freeNodes[nodesLeft - 2]->weight,
                freeNodes[nodesLeft - 2],
                freeNodes[nodesLeft - 1]
                );
        --nodesLeft;
        freeNodes[nodesLeft - 1] = combined;
    }

    // Final root is on the top
    return freeNodes[0];
}

void buildHTreeFromString(Node* HTreeRoot, const char* stringifiedHTree, int length, int* curPos) {
    while (*curPos < length) {
        if (stringifiedHTree[*curPos] == '0') {
            if (HTreeRoot->left == NULL) {
                HTreeRoot->left = newNode(0,0,0,0,0);
                (*curPos)++;
                buildHTreeFromString(HTreeRoot->left, stringifiedHTree, length, curPos);
            } else {
                return;
            }
        } else if (stringifiedHTree[*curPos] == '1') {
            if (HTreeRoot->right == NULL) {
                HTreeRoot->right = newNode(0,0,0,0,0);
                (*curPos)++;
                buildHTreeFromString(HTreeRoot->right, stringifiedHTree, length, curPos);
            } else {
                return;
            }
        } else {
            HTreeRoot->isLeaf = 1;
            HTreeRoot->byte = stringifiedHTree[*curPos];
            (*curPos)++;
            return;
        }
    }
}