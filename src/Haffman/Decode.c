#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "Decode.h"
#include "../SharedTypes.h"
#include "Tree/Node.h"
#include "Tree/BuildTree.h"
#include "Tree/StringifyHTree.h"
#include "BitReader/BitReader.h"

void decode(char* inputFilename, char* outputFilename) {
    // Try to open the file
    FILE* inputFile = fopen(inputFilename, "rb");
    if (!inputFile) {
        printf("Can't open the file %s\n", inputFilename);
        return;
    }

    // Get the length of the file
    fseek(inputFile, 0L, SEEK_END);
    uInt bytesInFile = ftell(inputFile);
    if (bytesInFile == 0) {
        printf("EMPTY FILE: Zero bytes in file\nNothing to decode\n");
        return;
    }

    // Print bytes of the inputFile
//    fseek(inputFile, 0L, SEEK_SET);
//    for (int byteIndex = 0; byteIndex < bytesInFile; ++byteIndex) {
//        uChar curByte = fgetc(inputFile);
//        for (int bitIdx = 7; bitIdx > -1; --bitIdx) {
//            printf("%d", (curByte >> bitIdx) & 1u);
//        }
//        printf("%c", (byteIndex % 2 ? '\n' : ' '));
//    }
//    printf("\n");

    // Move cursor to the beginning of the inputFile
    fseek(inputFile, 0L, SEEK_SET);

    // Get the number of trailing trash bits
    uInt trashTailBits = fgetc(inputFile);

    // Get the length of HTreeRepresentation
    uInt reprSize = (int)fgetc(inputFile) * 256;
    reprSize += fgetc(inputFile);

    // Get the HTreeRepresentation
    uChar* HTreeRepr = (uChar*)calloc(reprSize, sizeof(uChar));
    for (int byteIndex = 0; byteIndex < reprSize; ++byteIndex)
        HTreeRepr[byteIndex] = fgetc(inputFile);

    // Build HTree from repr
    Node* HTree = newNode(0, 0, 0, 0, 0, 0);
    buildHTreeFromByteRepr(HTree, HTreeRepr, reprSize);

    // Open output file
    FILE* outputFile = fopen(outputFilename, "wb");
    if (!outputFile) {
        printf("Can't open the file %s\n", outputFilename);
        return;
    }

    // Decode and output
    {
        unsigned long long bitsToRead = (bytesInFile - 3uLL - reprSize) * 8uLL - trashTailBits;
        BitReader *bitReader = newBitReader(inputFile, bitsToRead);
        Node* curNode = HTree;
        while (bitReader->haveBit(bitReader)) {
            uInt curBit = bitReader->readBit(bitReader);
            if (curBit == 0) {
                curNode = curNode->left;
                if (curNode->isLeaf) {
                    fputc(curNode->byte, outputFile);
                    curNode = HTree;
                }
            } else {
                curNode = curNode->right;
                if (curNode->isLeaf) {
                    fputc(curNode->byte, outputFile);
                    curNode = HTree;
                }
            }
        }
    }
}
/*
77:M = b1 (1)
86:V = b0 (1)
01010101 10101001
10100000
Trash Tailing Bits: 6
 */