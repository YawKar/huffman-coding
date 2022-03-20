#include "BytePrinter.h"
#include <stdlib.h>

void pushBit(BytePrinter* bytePrinter, uInt bit) {
    if (bytePrinter->currentBytePos == -1) {
        ++bytePrinter->printedBytes;
        fputc(bytePrinter->currentByte, bytePrinter->outputFile);
        bytePrinter->currentByte = 0;
        bytePrinter->currentBytePos = 7;
    }
    if (bit) {
        bytePrinter->currentByte |= ((uChar)1) << bytePrinter->currentBytePos;
    }/* else {
        bytePrinter->currentByte &= ~(((uChar)1) << bytePrinter->currentBytePos);
    }*/
    --bytePrinter->currentBytePos;
}

void dropLast(BytePrinter* bytePrinter) {
    if (bytePrinter->currentBytePos != 7) {
        fputc(bytePrinter->currentByte, bytePrinter->outputFile);
        ++bytePrinter->printedBytes;
        // currentBytePos is less than or equal to 6
        bytePrinter->defectBitsNumber = bytePrinter->currentBytePos + 1;
        bytePrinter->currentBytePos = 7;
        bytePrinter->currentByte = 0;
    }
}

BytePrinter* newBytePrinter(FILE* outputFile) {
    BytePrinter* bytePrinter = (BytePrinter*)calloc(1, sizeof(BytePrinter));
    bytePrinter->outputFile = outputFile;
    bytePrinter->currentByte = 0;
    bytePrinter->currentBytePos = 7;
    bytePrinter->printedBytes = 0;
    bytePrinter->defectBitsNumber = 0;
    bytePrinter->pushBit = &pushBit;
    bytePrinter->dropLast = &dropLast;
    return bytePrinter;
}