#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <unistd.h>
#include "define_para.h"
#include <signal.h>

#include "read.h"


int readCharArray(input_char_array_t *inputCharArray) {
    char buffer[1024];
    int printIndex = 0;
    int now = 0;

    int rtn = 0;
    bool ignoreSpace = true;
    char inputChar;
    int inputCharNowIndex = 0;

    bool singleQuote = false;
    bool doubleQuote = false;
    bool continueInput = false;
    while (true) {
        read(STDOUT_FILENO, &inputChar, 1);
        if ((int)inputChar == 4) {
            if (now == 0) {
                printf("\n");
                return 1;
            }
        }
        else if ((int)inputChar == 127) {
            now--;
            if (now < 0) now = 0;
        }
        else if ((int)inputChar == 3) {
            printf("\n");
            now = 0;
            return -1;
        }
        else {
            buffer[now++] = inputChar;
        }

        if (singleQuote == false && doubleQuote == false) {
            if (inputChar == '\'') {
                singleQuote = true;
            }
            if (inputChar == '\"') {
                doubleQuote = true;
            }
        }
        else {
            if (inputChar == '\'') {
                singleQuote = false;
            }
            if (inputChar == '\"') {
                doubleQuote = false;
            }
        }


        printf("\r\33[K");
        if (continueInput) {
            printf("> ");
        }
        else {
            printf("ve482sh $ ");
        }
        int i;
        for (i = printIndex; i < now; i++) {
            printf("%c", buffer[i]);
        }
        fflush(stdout);


        if (inputChar == '\n') {
            if (doubleQuote == false && singleQuote == false) {
                break;
            }
            else {
                continueInput = true;
                printIndex = now;
            }
        }
        printf("\r\33[K");
        if (continueInput) {
            printf("> ");
        }
        else {
            printf("ve482sh $ ");
        }
        for (i = printIndex; i < now; i++) {
            printf("%c", buffer[i]);
        }
        fflush(stdout);
    }
    int tmp = 0;
    now = 0;
    char outputbuffer[1024];

    singleQuote = false;
    doubleQuote = false;
    while (true) {
        if (singleQuote == false && doubleQuote == false ) {
            if (buffer[tmp] == '\'') {
                singleQuote = true;
            }
            if (buffer[tmp] == '\"') {
                doubleQuote = true;
            }
            if (buffer[tmp] == '<') {
                outputbuffer[now++] = ' ';
                outputbuffer[now++] = '<';
                outputbuffer[now++] = ' ';
            }
            else if (buffer[tmp] == '|') {
                outputbuffer[now++] = ' ';
                outputbuffer[now++] = '|';
                outputbuffer[now++] = ' ';
            }
            else if (buffer[tmp] == '>'){
                if (buffer[tmp + 1] == '>') {
                    outputbuffer[now++] = ' ';
                    outputbuffer[now++] = '>';
                    outputbuffer[now++] = '>';
                    outputbuffer[now++] = ' ';
                    tmp++;
                }
                else {
                    outputbuffer[now++] = ' ';
                    outputbuffer[now++] = '>';
                    outputbuffer[now++] = ' ';
                }
            }
            else if (buffer[tmp] == '\n') {
                outputbuffer[now++] = buffer[tmp];
                break;
            }
            else {
                outputbuffer[now++] = buffer[tmp];
            }
        }
        else {
            if (singleQuote && buffer[tmp] == '\'') {
                outputbuffer[now++] = buffer[tmp];
                singleQuote = false;
            }
            else if (doubleQuote && buffer[tmp] == '\"') {
                outputbuffer[now++] = buffer[tmp];
                doubleQuote = false;
            }
            else {
                outputbuffer[now++] = buffer[tmp];
            }
        }
        tmp++;
    }
    tmp = 0;
    singleQuote = false;
    doubleQuote = false;
    while(true){
        inputChar = outputbuffer[tmp++];
        if (singleQuote == false && doubleQuote == false) {
            if (inputChar == '\'') {
                singleQuote = true;
            }
            else if (inputChar == '\"') {
                doubleQuote = true;
            }
            else if (inputChar == '\n') {
                //printf("ogay!");
                if (!ignoreSpace) {
                    inputCharArray->charArray[inputCharArray->charNum][inputCharNowIndex] = '\0';
                    inputCharArray->charNum++;
                    inputCharNowIndex = 0;
                }
                return rtn;
            }
            else if (inputChar == ' ' && !ignoreSpace) {
                inputCharArray->charArray[inputCharArray->charNum][inputCharNowIndex] = '\0';
                inputCharArray->charNum++;
                inputCharNowIndex = 0;
                ignoreSpace = true;
            }
            else if (inputChar != ' ') {
                if (inputCharArray->charNum >= MAX_ARG_NUM) {
                    // TODO
                    return rtn;
                }
                ignoreSpace = false;
                inputCharArray->charArray[inputCharArray->charNum][inputCharNowIndex] = inputChar;
                inputCharNowIndex++;
                if (inputCharNowIndex >= MAX_ARG_LEN) {
                    // TODO
                    return rtn;
                }
            }
        }
        else {
            ignoreSpace = false;
            //printf("num: %d\n", inputCharArray->charNum);
            if (singleQuote && inputChar == '\'') {
                singleQuote = false;
            }
            else if (doubleQuote && inputChar == '\"') {
                doubleQuote = false;
            }
            else {
                if (inputCharArray->charNum >= MAX_ARG_NUM) {
                    // TODO
                    return rtn;
                }
                inputCharArray->charArray[inputCharArray->charNum][inputCharNowIndex] = inputChar;
                inputCharNowIndex++;
                if (inputCharNowIndex >= MAX_ARG_LEN) {
                    // TODO
                    return rtn;
                }
            }
        }


    }
    int i;
    for (i = 0; i < inputCharArray->charNum; i++) {
        printf("%s", inputCharArray->charArray[inputCharArray->charNum]);
    }
    return rtn;
}

int initCharArray(input_char_array_t *inputCharArray) {
    int rtn = 0;
    inputCharArray->charNum = 0;
    int i;
    for (i = 0; i < MAX_ARG_NUM; i++) {
        inputCharArray->charArray[i] = (char*)malloc(MAX_ARG_LEN);
    }
    return rtn;
}

int clearCharArray(input_char_array_t *inputCharArray) {
    int rtn = 0;
    int i;
    for (i = 0; i < MAX_ARG_NUM; i++) {
        free(inputCharArray->charArray[i]);
    }
    return rtn;
}

bool isCharArrayEmpty(input_char_array_t *inputCharArray) {
    if (inputCharArray->charNum == 0) {
        return true;
    }
    return false;
}
