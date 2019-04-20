#ifndef DISASM_H_INCLUDED
#define DISASM_H_INCLUDED
#include <iostream>
#include <cstdio>
#include "asm.h"
#include "cpu.h"

using namespace std;

bool Disasm_Program (FILE* in, FILE* out);
bool Create_Labels (int num_strings, int* cells, int** labels);
bool Disasm_String (FILE* out, int cell);

#endif // DISASM_H_INCLUDED
