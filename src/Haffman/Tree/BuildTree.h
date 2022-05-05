#ifndef HUFFMAN_CODING_BUILDTREE_H
#define HUFFMAN_CODING_BUILDTREE_H
#include "Node.h"
#include "../../SharedTypes.h"
Node* buildTree(const uInt* occurrencesCount);
void buildHTreeFromString(Node* HTreeRoot, const char* stringifiedHTree, int length, int* curPos);
void buildHTreeFromByteRepr(Node* HTreeRoot, uChar* repr, uInt reprSize);
#endif //HUFFMAN_CODING_BUILDTREE_H
