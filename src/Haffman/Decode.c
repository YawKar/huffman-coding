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

    // Move cursor to the beginning of the file
    fseek(inputFile, 0L, SEEK_SET);

    // Get the first byte that stores "uniqueBytes - 1" (we use "-1" in order to fit into a byte)
    uInt uniqueBytes = fgetc(inputFile) + 1;

    // Debug: print uniqueBytes
    printf("uniqueBytes = %d\n", uniqueBytes);

    // Get the stringified HTree
    char* stringifiedHTree = (char*)calloc(1, sizeof(char));
    {
        uInt encountered = 0;
        uInt strLen = 0;
        uInt strCap = 1;
        while (encountered != uniqueBytes) {
            uChar curChar = fgetc(inputFile);
            if (strLen == strCap - 1) {
                char* newStr = (char*)calloc(strCap * 2 + 1, sizeof(char));
                for (int i = 0; i < strLen; ++i)
                    newStr[i] = stringifiedHTree[i];
                free(stringifiedHTree);
                stringifiedHTree = newStr;
                strCap = strCap * 2 + 1;
            }
            stringifiedHTree[strLen++] = (char)curChar;
            if (curChar != '1' && curChar != '0')
                ++encountered;
        }
    }

    // Debug: print stringifiedHTree
    printf("stringifiedHTree = %s\n", stringifiedHTree);

    // Build HTree from the stringrepr
    Node* HTreeRoot = newNode(0, 0, 0, 0, 0);
    int curPos = 0;
    buildHTreeFromString(HTreeRoot, stringifiedHTree, strlen(stringifiedHTree), &curPos);

    // Calculate the body (minus: 1 first byte, stringified Htree, last byte);
    uInt bytesInBody = bytesInFile - 1 - strlen(stringifiedHTree) - 1;

    // Get the last byte of defect bits
    fseek(inputFile, (long)bytesInFile - 1, SEEK_SET);
    uInt defectBitsNumber = fgetc(inputFile);

    // Return cursor to the beginning
    fseek(inputFile, 0L, SEEK_SET);

    // Create cursor in HTree
    Node* cursor = HTreeRoot;

    // Open output file
    FILE* outputFile = fopen(outputFilename, "wb");
    if (!outputFile) {
        printf("Can't open the file %s\n", outputFilename);
        return;
    }

    // Create BitReader instance
    fseek(inputFile, bytesInFile - 1 - bytesInBody, SEEK_SET);
    BitReader* bitReader = newBitReader(inputFile, bytesInBody);
    for (long i = 0; i < (long)bytesInBody * 8 - defectBitsNumber; ++i) {
        uInt curBit = bitReader->readBit(bitReader);
        if (curBit) {
            cursor = cursor->right;
        } else {
            cursor = cursor->left;
        }
        if (cursor->isLeaf) {
            //printf("%c", cursor->byte);
            fputc(cursor->byte, outputFile);
            cursor = HTreeRoot;
        }
    }

}