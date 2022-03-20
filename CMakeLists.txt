cmake_minimum_required(VERSION 3.21)
project(huffman_coding C)

set(CMAKE_C_STANDARD 11)

add_executable(huffman_coding
        src/main.c src/CONFIG.h src/Haffman/Encode.c src/Haffman/Encode.h src/Haffman/Decode.c src/Haffman/Decode.h src/SharedTypes.h src/Haffman/Tree/Node.c src/Haffman/Tree/Node.h src/Haffman/Tree/BuildTree.c src/Haffman/Tree/BuildTree.h src/Haffman/Tree/ExtractCodes.c src/Haffman/Tree/ExtractCodes.h src/Haffman/Tree/StringifyHTree.c src/Haffman/Tree/StringifyHTree.h src/Haffman/BytePrinter/BytePrinter.c src/Haffman/BytePrinter/BytePrinter.h src/Haffman/BitReader/BitReader.c src/Haffman/BitReader/BitReader.h)
