#ifndef PARSE_H
#define PARSE_H

#include "define_para.h"
#include "read.h"

typedef struct command_t command_t;
typedef struct pipe_t pipe_t;
typedef struct fork_t fork_t;

typedef char* command;

enum IOMODE {
	STDIN, STDOUT, FILEIN, FILEOUT, APPEND
};

static char* IOMODEchar[5] = {"stdin", "stdout", "filein", "fileout", "append"};

struct command_t{
	int argc;
    command argv[MAX_ARG_NUM];
	char* inputFile;
	char* outputFile;
	enum IOMODE inputMode;
	enum IOMODE outputMode;
};

struct pipe_t {
	int pipeNum;
	command_t pipeCommand[MAX_PIPE_NUM];
};


struct fork_t {
	int forkNum;
	int forkpid[MAX_PIPE_NUM];
};


int parseCommand(input_char_array_t *inputCharArray, pipe_t *wholePipeLine);

void initCommand(command_t* parsedCommand);

#endif //PARSE_H
