#ifndef HUFFMAN_CODING_BITREADER_H
#define HUFFMAN_CODING_BITREADER_H

#include <stdio.h>
#include "../../SharedTypes.h"

typedef struct BitReader {
    FILE* inputFile;
    uChar currentByte;
    uInt currentBitPos;
    uInt countReadBytes;
    uInt bytesLeft;
    uInt (*readBit) (struct BitReader*);
} BitReader;

BitReader* newBitReader(FILE* inputFile, uInt numberOfBytesToRead);

#endif //HUFFMAN_CODING_BITREADER_H