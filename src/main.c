#include <stdio.h>
#include <string.h>

#include "CONFIG.h"
#include "Haffman/Decode.h"
#include "Haffman/Encode.h"

void application() {
    char inputfile[256];
    char outputfile[256];
    char mode[20];
    printf("Mode <<<");
    scanf("%s", mode);
    printf("Input filename <<<");
    scanf("%s", inputfile);
    printf("Output filename <<<");
    scanf("%s", outputfile);
    if (strcmp(mode, "encode") == 0)
        encode(inputfile, outputfile);
    else if (strcmp(mode, "decode") == 0)
        decode(inputfile, outputfile);
    else
        printf("Unknown command\n");
}

void play() {

}

int main() {
    #ifdef MAIN
    application();
    #endif
    #ifdef PLAY
    play();
    #endif
}