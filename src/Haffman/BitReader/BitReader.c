#include <stdlib.h>
#include "BitReader.h"

uInt readBit(BitReader* bitReader) {
    if (bitReader->currentBitPos == -1) {
        bitReader->currentByte = fgetc(bitReader->inputFile);
        bitReader->currentBitPos = 7;
        bitReader->countReadBytes++;
    }
    return (bitReader->currentByte >> (bitReader->currentBitPos--)) & 1;
}

BitReader* newBitReader(FILE* inputFile, uInt numberOfBytesToRead) {
    BitReader* bitReader = (BitReader*)calloc(1, sizeof(BitReader));
    bitReader->inputFile = inputFile;
    bitReader->currentByte = 0;
    bitReader->currentBitPos = -1;
    bitReader->bytesLeft = numberOfBytesToRead;
    bitReader->countReadBytes = 0;
    bitReader->readBit = &readBit;
    return bitReader;
}