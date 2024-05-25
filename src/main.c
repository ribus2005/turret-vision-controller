#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <string.h>

#include "turrel-interface.h"

int main(int argc, char** argv) {
    if (argc > 2) {
        printf("bro you could just type '_'\n");
        printf("anyway\n");
        printf("\n");
    }

    char* serialPort = argv[1];

    printWelcomeMessage();

    loopCmd(serialPort);

    return 0;
}