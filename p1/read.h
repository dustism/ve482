#ifndef READ_H
#define READ_H

#include <stdbool.h>

#include "define_para.h"

typedef struct input_char_array_t input_char_array_t;

struct input_char_array_t{
	char* charArray[MAX_ARG_NUM];
	int charNum;
};

int readCharArray(input_char_array_t *inputCharArray);

int initCharArray(input_char_array_t *inputCharArray);

int clearCharArray(input_char_array_t *inputCharArray);

bool isCharArrayEmpty(input_char_array_t *inputCharArray);

#endif //READ_H
