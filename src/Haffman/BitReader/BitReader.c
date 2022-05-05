#include <stdlib.h>
#include "BitReader.h"

uInt readBit(BitReader* bitReader) {
    if (bitReader->currentBitIdx == -1) {
        bitReader->currentBitIdx = 7;
        bitReader->currentByte = fgetc(bitReader->inputFile);
    }
    bitReader->readBitsCounter++;
    uInt kek = (bitReader->currentByte >> bitReader->currentBitIdx) & 1u;
    bitReader->currentBitIdx--;
    return kek;
}

uInt haveBit(BitReader* bitReader) {
    return bitReader->readBitsCounter < bitReader->bitsToReadTotal;
}

BitReader* newBitReader(FILE* inputFile, unsigned long long bitsToReadTotal) {
    BitReader* bitReader = (BitReader*)calloc(1, sizeof(BitReader));
    bitReader->inputFile = inputFile;
    bitReader->bitsToReadTotal = bitsToReadTotal;
    bitReader->readBit = &readBit;
    bitReader->haveBit = &haveBit;
    bitReader->currentByte = 0;
    bitReader->currentBitIdx = -1;
    bitReader->readBitsCounter = 0;
    return bitReader;
}