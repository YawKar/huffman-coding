#ifndef HUFFMAN_CODING_NODE_H
#define HUFFMAN_CODING_NODE_H
#include "../../SharedTypes.h"

typedef struct Node {
    struct Node *left, *right;
    uInt weight;
    uChar byte;
    uChar isLeaf;
} Node;

Node* newNode(uChar byte, uChar isLeaf, uInt weight, Node* left, Node* right);
#endif //HUFFMAN_CODING_NODE_H
