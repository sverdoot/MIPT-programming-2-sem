#ifndef ASM2_0_H_INCLUDED
#define ASM2_0_H_INCLUDED
#include <iostream>
#include <map>
#include <string>
#include "commands.h"

using namespace std;

bool Read_Asm_String (char* &asm_text, int* &ready_code, map <string, int> &Labels);
int* Read_Asm_File (FILE* file_asm);
int Read_Command (char* &asm_text);
int Get_Int (char* &asm_text);
int Read_Labels (char* buf, map <string, int> &Labels);

const size_t CELL_SIZE = 32;
const size_t COMMAND_SIZE = 8;
const size_t NUM_REG_SIZE = 4;
const size_t IM_CONST_SIZE = 20;
const size_t IM_MOD_SIZE = 16;
const int MAX_ASM_SIZE = 100000;
const int MAX_STRING_SIZE = 1000;
const int L_CAPACITY = 1000;
const int MAX_LABEL_LENGHT = 20;
const int FAIL = -1;

#endif // ASM2_0_H_INCLUDED
