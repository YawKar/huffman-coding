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
                freeNodes[nodeIndex++] = newNode(byte, 1, occurrencesCount[byte], NULL, NULL, NULL);
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
                freeNodes[nodesLeft - 1],
                NULL);
        freeNodes[nodesLeft - 2]->parent = combined;
        freeNodes[nodesLeft - 1]->parent = combined;
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
                HTreeRoot->left = newNode(0,0,0,0,0,0);
                (*curPos)++;
                buildHTreeFromString(HTreeRoot->left, stringifiedHTree, length, curPos);
            } else {
                return;
            }
        } else if (stringifiedHTree[*curPos] == '1') {
            if (HTreeRoot->right == NULL) {
                HTreeRoot->right = newNode(0,0,0,0,0,0);
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

void buildHTreeFromByteRepr(Node* HTreeRoot, uChar* repr, uInt reprSize) {
    Node* curNode = HTreeRoot;
    uInt curBitIdx = 7;
    uInt curByteIdx = 0;

    uChar curBit = (repr[curByteIdx] >> curBitIdx) & 1u;
    --curBitIdx;
    while (1) {
        if (curBit) {
            // now we'll encounter symbol
            // if curNode is a leaf
            if (curNode->left == 0 && curNode->right == 0) {
                // get 8 bits that denote an ASCII char
                uChar symbol = 0;
                if (curBitIdx == -1) {
                    ++curByteIdx;
                    curBitIdx = 7;
                }
                for (int i = 7; i > -1; --i) {
                    uChar bit = (repr[curByteIdx] >> curBitIdx) & 1u;
                    --curBitIdx;
                    if (bit)
                        symbol |= (bit << i);
                    if (curBitIdx == -1) {
                        ++curByteIdx;
                        curBitIdx = 7;
                    }
                }
                curBit = (repr[curByteIdx] >> curBitIdx) & 1u;
                --curBitIdx;
                if (curBitIdx == -1) {
                    ++curByteIdx;
                    curBitIdx = 7;
                }
                curNode->isLeaf = 1;
                curNode->byte = symbol;
                curNode = curNode->parent;
                // stop if it was the last one
                if (curByteIdx == reprSize)
                    break;
            } else if (curNode->right == 0) {
                // go right
                curNode->right = newNode(0, 0, 0, 0, 0, curNode);
                curNode = curNode->right;
            } else {
                // go up
                if (curNode == HTreeRoot)
                    // Stop: there's the trash tail
                    break;
                curNode = curNode->parent;
            }
        } else {
            // just a move
            if (curNode->left == 0) {
                curNode->left = newNode(0, 0, 0, 0, 0, curNode);
                curNode = curNode->left;
                if (curBitIdx == -1) {
                    ++curByteIdx;
                    curBitIdx = 7;
                }
                curBit = (repr[curByteIdx] >> curBitIdx) & 1u;
                --curBitIdx;
            } else if (curNode->right == 0) {
                curNode->right = newNode(0, 0, 0, 0, 0, curNode);
                curNode = curNode->right;
            } else {
                if (curNode == HTreeRoot)
                    // Stop: there's the trash tail
                    break;
                curNode = curNode->parent;
            }
        }
    }
}