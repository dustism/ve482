#include <stdbool.h>
#include <string.h>
#include <unistd.h>
#include <stdio.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <signal.h>
#include <stdlib.h>

#include "execute.h"

static char pwd_buffer[MAX_ARG_LEN + 1] = {0};


int cd(char *path) {
    return chdir(path);
}

int executeCommand(int pipeIn, int pipeOut, command_t *parsedCommand, bool lastCommand, int *forkpid) {
    pid_t child_pid;
    int stat_loc;
    child_pid = fork();
    *forkpid = child_pid;
    if (child_pid == 0) {
        int fd_in, fd_out;
        int rtn;

        if (pipeIn != 0) {
            dup2(pipeIn, 0);
            close(pipeIn);
        }
        if (pipeOut != 1) {
            dup2(pipeOut, 1);
            close(pipeOut);
        }
        if (parsedCommand->inputMode == FILEIN) {
            fd_in = open(parsedCommand->inputFile, O_RDONLY);
            if (fd_in > 0) {
                dup2(fd_in, 0);
            }
            else {
                rtn = -1;
                printf("file io error! error message: open file failed! file name: %s\n", parsedCommand->inputFile);
                return rtn;
            }
        }
        if (parsedCommand->outputMode == FILEOUT) {
            fd_out = open(parsedCommand->outputFile, O_WRONLY | O_CREAT | O_TRUNC);
            if (fd_out > 0) {
                dup2(fd_out, 1);
            }
            else {
                rtn = -1;
                printf("file io error! error message: open file failed! file name: %s\n", parsedCommand->outputFile);
                return rtn;
            }
        }
        if (parsedCommand->outputMode == APPEND) {
            fd_out = open(parsedCommand->outputFile, O_APPEND | O_CREAT | O_WRONLY);
            if (fd_out > 0) {
                dup2(fd_out, 1);
            }
            else {
                rtn = -1;
                printf("file io error! error message: open file failed! file name: %s\n", parsedCommand->outputFile);
                return rtn;
            }
        }
        if (strcmp(parsedCommand->argv[0], "pwd") == 0) {

            char *result = getcwd(pwd_buffer, MAX_ARG_LEN);

            printf("%s\n", pwd_buffer);
        }
        else if (execvp(parsedCommand->argv[0], parsedCommand->argv) < 0) {
            perror(parsedCommand->argv[0]);
            rtn = -1;
            exit(0);
        }
        if (parsedCommand->inputMode == FILEIN) {
            close(fd_in);
        }
        if (parsedCommand->outputMode == FILEOUT || parsedCommand->outputMode == APPEND) {
            close(fd_out);
        }
        if (strcmp(parsedCommand->argv[0], "pwd") == 0) {

            exit(0);
        }

    }
    else {
        if (lastCommand) waitpid(child_pid, &stat_loc, WUNTRACED);
    }
    return 0;
}


int forkPipes(pipe_t *wholePipeLine, bool *exitState, fork_t *myFork) {
    pid_t pid;
    int pipeIn = 0, fd [2];
    myFork->forkNum = 0;

    int pipeIndex;
    for (pipeIndex = 0; pipeIndex < wholePipeLine->pipeNum - 1; pipeIndex++) {
        *exitState = false;
        int rtn = 0;
        if (strcmp(wholePipeLine->pipeCommand[pipeIndex].argv[0], "exit") == 0) {
            *exitState = true;
            return rtn;
        }

        if (strcmp(wholePipeLine->pipeCommand[pipeIndex].argv[0], "cd") == 0) {
            if (cd(wholePipeLine->pipeCommand[pipeIndex].argv[1]) < 0) {
                perror(wholePipeLine->pipeCommand[pipeIndex].argv[0]);
            }
            return rtn;
        }

        //fork procedure
        pipe(fd);

        rtn = executeCommand(pipeIn, fd[1], ((wholePipeLine->pipeCommand) + pipeIndex), false, &(myFork->forkpid[myFork->forkNum]));
        myFork->forkNum++;
        if (rtn != 0) {
            return rtn;
        }
        close(fd[1]);
        pipeIn = fd[0];
    }
    *exitState = false;
    int rtn = 0;
    if (strcmp(wholePipeLine->pipeCommand[wholePipeLine->pipeNum - 1].argv[0], "exit") == 0) {
        *exitState = true;
        return rtn;
    }

    if (strcmp(wholePipeLine->pipeCommand[wholePipeLine->pipeNum - 1].argv[0], "cd") == 0) {
        if (cd(wholePipeLine->pipeCommand[wholePipeLine->pipeNum - 1].argv[1]) < 0) {
            perror(wholePipeLine->pipeCommand[wholePipeLine->pipeNum - 1].argv[0]);
        }
        return rtn;
    }
    rtn = executeCommand(pipeIn, 1, &(wholePipeLine->pipeCommand[wholePipeLine->pipeNum - 1]), true, &(myFork->forkpid[myFork->forkNum]));
    myFork->forkNum++;
    if (rtn != 0) {
        return rtn;
    }
    return 0;
}
