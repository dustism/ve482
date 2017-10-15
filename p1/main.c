#include <stdio.h>
#include <stdbool.h>

#include "read.h"
#include "parse.h"
#include "execute.h"
#include "define_para.h"
#include <sys/wait.h>
#include <sys/stat.h>
#include <signal.h>
#include <termios.h>
#include <unistd.h>

static struct termios origin_termios;
fork_t myFork;

void sighandler(int signum) {
    int i;
    for (i = 0; i < myFork.forkNum; i++) {
        kill(myFork.forkpid[i], SIGABRT);
        // kill all the process unfinished
    }
    printf("\n");
}


int main() {
    signal(SIGINT, sighandler);
    while (true) {
        printf("ve482sh $ ");
        fflush(stdout);
        input_char_array_t inputCharArray;
        int rtn;
        rtn = initCharArray(&inputCharArray);


        int rs = tcgetattr(STDIN_FILENO, &origin_termios);
        struct termios editor_termios = origin_termios;
        editor_termios.c_lflag &= ~(ECHO | ICANON | ISIG);
        rs = tcsetattr(STDIN_FILENO, TCSAFLUSH, &editor_termios);
        rtn = readCharArray(&inputCharArray);
        rs = tcsetattr(STDIN_FILENO, TCSAFLUSH, &origin_termios);

        if (rtn == 1) {
            rtn = clearCharArray(&inputCharArray);
            break;
        }
        if (rtn == -1) {
            rtn = clearCharArray(&inputCharArray);
            continue;
        }
        bool check;
        check = isCharArrayEmpty(&inputCharArray);
        if (check) {
            rtn = clearCharArray(&inputCharArray);
            continue;
        }
        pipe_t wholePipeLine;

        rtn = parseCommand(&inputCharArray, &wholePipeLine);

        if (rtn != 0) {
            rtn = clearCharArray(&inputCharArray);
            continue;
        }

        /*for (int i = 0; i < wholePipeLine.pipeNum; i++) {
            printf("pipe: %d\n", i);
            for (int j = 0; j < wholePipeLine.pipeCommand[i].argc; j++)
            printf("command %d: %s\n", j, wholePipeLine.pipeCommand[i].argv[j]);
            printf("%s\n", IOMODEchar[wholePipeLine.pipeCommand[i].inputMode]);
            printf("%s\n", IOMODEchar[wholePipeLine.pipeCommand[i].outputMode]);
            printf("\n");
        }*/

        bool exitState;
        rtn = forkPipes(&wholePipeLine, &exitState, &myFork);
        if (rtn != 0) {
            rtn = clearCharArray(&inputCharArray);
            continue;
        }
        rtn = clearCharArray(&inputCharArray);
        int i;
        for (i = 0; i < myFork.forkNum; i++) {
            kill(myFork.forkpid[i], SIGABRT);
            // kill all the process unfinished
        }


        if (exitState) {
            break;
        }
    }
    return 0;
}
