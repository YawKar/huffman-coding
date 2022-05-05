#include "StringifyHTree.h"
#include "../../SharedTypes.h"
#include <string.h>
#include <stdlib.h>

#include <stdio.h>

void stringifyHTree(Node* HTreeRoot, uChar* hash, int* size) {
    uChar count = 0;
    uChar buf = '\0';

    Node* cur_root = HTreeRoot;
    while (1) {
        if (cur_root->left != NULL && cur_root->left->used != 1) {
            buf |= 0 << (7 - count++);
            if (count == 8) {
                hash[(*size)++] = buf;
                count = 0;
                buf = '\0';
            }
            cur_root = cur_root->left;
            cur_root->used = 1;
        } else if (cur_root->right != NULL && cur_root->right->used != 1) {
            cur_root->used = 1;
            cur_root = cur_root->right;
        } else {
            if (cur_root->isLeaf) {
                buf |= 1 << (7 - count++);
                if (count == 8) {
                    hash[(*size)++] = buf;
                    count = 0;
                    buf = '\0';
                }
                uChar letter = cur_root->byte;
                for (int i = 0; i < 8; ++i) {
                    int bit = 1 & (letter >> (7 - i));
                    buf |= bit << (7 - count++);
                    if (count == 8) {
                        hash[(*size)++] = buf;
                        count = 0;
                        buf = '\0';
                    }
                }
            }
            cur_root->used = 1;
            cur_root = cur_root->parent;
            if (cur_root == NULL)
                break;
        }
    }
    hash[(*size)++] = buf;
}