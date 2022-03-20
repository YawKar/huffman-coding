#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "Encode.h"
#include "Tree/Node.h"
#include "Tree/BuildTree.h"
#include "Tree/ExtractCodes.h"
#include "../SharedTypes.h"
#include "Tree/StringifyHTree.h"
#include "BytePrinter/BytePrinter.h"

void encode(char* inputFilename, char* outputFilename) {
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
        printf("EMPTY FILE: Zero bytes in file\nNothing to encode\n");
        return;
    }

    // Count occurrences of each of the bytes in the file
    uInt* occurrencesCount = (uInt*)calloc(256, sizeof(uInt));
    fseek(inputFile, 0L, SEEK_SET);
    for (int byteIndex = 0; byteIndex < bytesInFile; ++byteIndex) {
        uChar curByte = fgetc(inputFile);
        ++occurrencesCount[curByte];
    }

    // Build Huffman Tree
    Node* HTreeRoot = buildTree(occurrencesCount);

    // Get codes from Huffman Tree
    char** codes = (char**)calloc(256, sizeof(char*));
    extractCodesFromHTree(HTreeRoot, codes, (char*)calloc(524, sizeof(char)));

    // Precalculate lengths of each of the codes
    uInt* codeLength = (uInt*)calloc(256, sizeof(uInt));
    for (int i = 0; i < 256; ++i) {
        if (occurrencesCount[i])
            codeLength[i] = strlen(codes[i]);
    }

    // Debug: print out codes, their meanings and lengths
    for (int i = 0; i < 256; ++i) {
        if (occurrencesCount[i]) {
            printf("%d:%c = b%s (%d)\n", i, i, codes[i], codeLength[i]);
        }
    }

    // Get the string representation of the HTreeRoot
    char* strReprHTree = (char*)calloc(1, sizeof(char));
    uInt curCapacity = 1;
    stringifyHTree(HTreeRoot, &strReprHTree, &curCapacity);

    // Debug: print out the stringified HTree
    printf("%s\n", strReprHTree);

    // Count how many unique bytes are there
    uInt uniqueBytes = 0;
    for (int i = 0; i < 256; ++i)
        uniqueBytes += occurrencesCount[i] != 0;

    // PRINTING PART

    // Open the output file
    FILE* outputFile = fopen(outputFilename, "wb");
    if (!outputFile) {
        printf("Unable to open/create output file!\n");
        return;
    }
    // Print the first byte that stores "uniqueBytes - 1" (we use "-1" in order to fit into a byte)
    fputc((int)uniqueBytes - 1, outputFile);
    // Print the stringified HTree
    fputs(strReprHTree, outputFile);
    // Print the input file content via codes
    fseek(inputFile, 0L, SEEK_SET);
    BytePrinter* bytePrinter = newBytePrinter(outputFile);
    for (uInt byteIndex = 0; byteIndex < bytesInFile; ++byteIndex) {
        uChar curByte = fgetc(inputFile);
        for (uInt bitIndex = 0; bitIndex < codeLength[curByte]; ++bitIndex) {
            bytePrinter->pushBit(bytePrinter, codes[curByte][bitIndex] == '1');
        }
    }
    bytePrinter->dropLast(bytePrinter);
    // Print the number of defect bits (bits without meaning in the end)
    fputc((int)bytePrinter->defectBitsNumber, outputFile);
}