#include <stdio.h>
#include <string.h>

#include "parse.h"

void initCommand(command_t* parsedCommand) {
    parsedCommand->inputMode = STDIN;
    parsedCommand->outputMode = STDOUT;
    parsedCommand->argc = 0;
    return;
}

int parseCommand(input_char_array_t *inputCharArray, pipe_t *wholePipeLine) {
    int rtn = 0;
    int index = 0;
    wholePipeLine->pipeNum = 0;
    initCommand(&(wholePipeLine->pipeCommand[0]));
    while (index < inputCharArray->charNum) {
        if (strcmp(inputCharArray->charArray[index], "|") == 0) {
            wholePipeLine->pipeCommand[wholePipeLine->pipeNum].argv[wholePipeLine->pipeCommand[wholePipeLine->pipeNum].argc] = NULL;
            wholePipeLine->pipeNum++;
            initCommand(&(wholePipeLine->pipeCommand[wholePipeLine->pipeNum]));
        }
        else if (strcmp(inputCharArray->charArray[index], "<") == 0) {
            if (wholePipeLine->pipeCommand[wholePipeLine->pipeNum].inputMode == FILEIN) {
                rtn = -1;
                printf("redirection error! error message: multi redirection for input!\n");
                return rtn;
            }
            if  (index + 1 < inputCharArray->charNum) {
                wholePipeLine->pipeCommand[wholePipeLine->pipeNum].inputMode = FILEIN;
                index++;
                wholePipeLine->pipeCommand[wholePipeLine->pipeNum].inputFile = inputCharArray->charArray[index];
            }
            else {
                rtn = -1;
                printf("redirection error! error message: no file for redirection!\n");
                return rtn;
            }
        }
        else if (strcmp(inputCharArray->charArray[index], ">") == 0) {
            if (wholePipeLine->pipeCommand[wholePipeLine->pipeNum].outputMode == FILEOUT || wholePipeLine->pipeCommand[wholePipeLine->pipeNum].outputMode == APPEND) {
                rtn = -1;
                printf("redirection error! error message: Duplicated redirection for output!\n");
                return rtn;
            }
            if  (index + 1 < inputCharArray->charNum) {
                wholePipeLine->pipeCommand[wholePipeLine->pipeNum].outputMode = FILEOUT;
                index++;
                wholePipeLine->pipeCommand[wholePipeLine->pipeNum].outputFile = inputCharArray->charArray[index];
            }
            else {
                rtn = -1;
                printf("redirection error! error message: no file for redirection!\n");
                return rtn;
            }
        }
        else if (strcmp(inputCharArray->charArray[index], ">>") == 0) {
            if (wholePipeLine->pipeCommand[wholePipeLine->pipeNum].outputMode == FILEOUT || wholePipeLine->pipeCommand[wholePipeLine->pipeNum].outputMode == APPEND) {
                rtn = -1;
                printf("redirection error! error message: Duplicated redirection to append output!\n");
                return rtn;
            }
            if  (index + 1 < inputCharArray->charNum) {
                wholePipeLine->pipeCommand[wholePipeLine->pipeNum].outputMode = APPEND;
                index++;
                wholePipeLine->pipeCommand[wholePipeLine->pipeNum].outputFile = inputCharArray->charArray[index];
            }
            else {
                rtn = -1;
                printf("redirection error! error message: no file for redirection!\n");
                return rtn;
            }
        }
        else {
            wholePipeLine->pipeCommand[wholePipeLine->pipeNum].argv[wholePipeLine->pipeCommand[wholePipeLine->pipeNum].argc] = inputCharArray->charArray[index];
            wholePipeLine->pipeCommand[wholePipeLine->pipeNum].argc++;
        }
        index++;
    }
    wholePipeLine->pipeCommand[wholePipeLine->pipeNum].argv[wholePipeLine->pipeCommand[wholePipeLine->pipeNum].argc] = NULL;
    wholePipeLine->pipeNum++;
    return rtn;
}
