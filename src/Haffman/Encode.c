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
    } else if (bytesInFile == 1) {
        printf("File must contain at least 2 bytes.\n");
        return;
    }

    // Count occurrences of each of the bytes in the file
    uInt* occurrencesCount = (uInt*)calloc(256, sizeof(uInt));
    fseek(inputFile, 0L, SEEK_SET);
    for (int byteIndex = 0; byteIndex < bytesInFile; ++byteIndex) {
        uChar curByte = fgetc(inputFile);
        ++occurrencesCount[curByte];
    }

    // Check if there's only one byte
    {
        uInt countTotalPresentBytes = 0;
        for (int i = 0; i < 256; ++i)
            countTotalPresentBytes += occurrencesCount[i] > 0;
        if (countTotalPresentBytes == 1) {
            // Add auxiliary byte
            for (int i = 0; i < 256; ++i) {
                if (occurrencesCount[i] == 0) {
                    occurrencesCount[i] = 1;
                    break;
                }
            }
        }
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
    int size = 0;
    uChar* hash = (uChar*)calloc(320, sizeof(uChar));
    stringifyHTree(HTreeRoot, hash, &size);

    // Debug: Print bits of the string representation
    for (int i = 0; i < size; ++i) {
        for (int bit = 7; bit > -1; --bit)
            printf("%d", 1 & (hash[i] >> bit));
        printf("%c", (i % 2 ? '\n' : ' '));
    }
    printf("\n");

    // Calculate the number of trash bits of the last byte
    uInt trashTailBits = 0;
    {
        uInt curReminder = 0;
        fseek(inputFile, 0L, SEEK_SET);
        for (int byteIndex = 0; byteIndex < bytesInFile; ++byteIndex) {
            uChar curByte = fgetc(inputFile);
            curReminder += codeLength[curByte];
            curReminder %= 8u;
        }
        trashTailBits = (8u - curReminder) % 8u;
    }

    // Print trashTailBits
    printf("Trash Tailing Bits: %d\n", trashTailBits);

    // ============== START ENCODING & OUTPUTING =================

    // Open outputFile
    FILE* outputFile = fopen(outputFilename, "wb");
    if (!outputFile) {
        printf("Can't open the file %s\n", outputFilename);
        return;
    }

    // Output 1 byte that stores trashTailBits
    fputc((char)trashTailBits, outputFile);

    // Output 2 bytes that store length of HTree Representation
    fputc((char)(size / 256), outputFile);
    fputc((char)(size % 256), outputFile);

    // Output HTree representation
    for (int byteIndex = 0; byteIndex < size; ++byteIndex) {
        fputc((char)hash[byteIndex], outputFile);
    }

    // Output the whole encoded
    {
        uChar curOutputByte = 0;
        uInt curPos = 7;
        fseek(inputFile, 0L, SEEK_SET);
        for (int byteIndex = 0; byteIndex < bytesInFile; ++byteIndex) {
            uChar curByte = fgetc(inputFile);
            // Output the binary code for the curByte
            for (int curBit = 0; curBit < codeLength[curByte]; ++curBit) {
                if (codes[curByte][curBit] == '1') {
                    curOutputByte |= (1u << curPos);
                }
                --curPos;
                if (curPos == -1) {
                    fputc(curOutputByte, outputFile);
                    curOutputByte = 0;
                    curPos = 7;
                }
            }
        }
        // last byte with trash tail
        if (curPos != 7) {
            fputc(curOutputByte, outputFile);
            curOutputByte = 0;
            curPos = 7;
        }
    }
}