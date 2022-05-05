#ifndef HUFFMAN_CODING_BITREADER_H
#define HUFFMAN_CODING_BITREADER_H

#include <stdio.h>
#include "../../SharedTypes.h"

typedef struct BitReader {
    FILE* inputFile;
    uChar currentByte;
    uInt currentBitIdx;
    uInt readBitsCounter;
    unsigned long long bitsToReadTotal;
    uInt (*readBit) (struct BitReader*);
    uInt (*haveBit) (struct BitReader*);
} BitReader;

BitReader* newBitReader(FILE* inputFile, unsigned long long bitsToReadTotal);

#endif //HUFFMAN_CODING_BITREADER_H