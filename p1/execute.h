#ifndef EXECUTE_H
#define EXECUTE_H

#include "parse.h"

int executeCommand(int pipeIn, int pipeOut, command_t *parsedCommand, bool lastCommand, int *forkpid);

int forkPipes(pipe_t *wholePipeLine, bool *exitState, fork_t *myFork);

int cd(char *path);



#endif //EXECUTE_H
