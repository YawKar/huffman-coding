#include <stdlib.h>

#include "Node.h"

Node* newNode(uChar byte, uChar isLeaf, uInt weight, Node* left, Node* right, Node* parent) {
    Node* node = (Node*)malloc(sizeof(Node));
    node->byte = byte;
    node->isLeaf = isLeaf;
    node->weight = weight;
    node->left = left;
    node->right = right;
    node->parent = parent;
    node->used = 0;
    return node;
}