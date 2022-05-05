#ifndef HUFFMAN_CODING_NODE_H
#define HUFFMAN_CODING_NODE_H
#include "../../SharedTypes.h"

typedef struct Node {
    struct Node *left, *right, *parent;
    uInt weight;
    uChar byte;
    uChar isLeaf;
    uChar used;
} Node;

Node* newNode(uChar byte, uChar isLeaf, uInt weight, Node* left, Node* right, Node* parent);
#endif //HUFFMAN_CODING_NODE_H
