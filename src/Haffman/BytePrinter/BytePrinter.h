#ifndef HUFFMAN_CODING_BYTEPRINTER_H
#define HUFFMAN_CODING_BYTEPRINTER_H
#include "../../SharedTypes.h"
#include <stdio.h>

typedef struct BytePrinter {
    FILE* outputFile;
    uChar currentByte;
    uInt currentBytePos;
    uInt printedBytes;
    uInt defectBitsNumber;
    void (*pushBit) (struct BytePrinter* bytePrinter, uInt bit);
    void (*dropLast) (struct BytePrinter* bytePrinter);
} BytePrinter;

BytePrinter* newBytePrinter(FILE* outputFile);
#endif //HUFFMAN_CODING_BYTEPRINTER_H
