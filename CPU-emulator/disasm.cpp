#include <iostream>
#include <cstdio>
#include "asm.h"
#include "cpu.h"
#include <cassert>
#include "disasm.h"
#include <string.h>
#include <map>

using namespace std;

bool Disasm_Program (FILE* in, FILE* out)
    {
    cout << "\n===Function \"Disasm_Program\" is active.===\n\n";

    assert (out);
    assert (in);

    char c;
    int size_of_code = 0;

    while (c != '\n')
        c = fgetc (in);
    fscanf (in, "%d", &size_of_code);

    int num_strings = size_of_code;
    int* cells = new int[num_strings];

    for (int i = 0; i < num_strings; i++)
        fscanf (in, "%d\n", &cells[i]);

    int* labels = new int[num_strings];
    Create_Labels (num_strings, cells, &labels);

    for (int i = 0; i < num_strings; i++)
        {
        if (labels[i] == 1)
            fprintf (out, "label_%d:\n", i);
        Disasm_String (out, cells[i]);
        }

    delete []labels;
    delete []cells;

    cout << "\n===Function \"Disasm_Program\" successfully executed.===\n\n";

    return true;
    }

bool Disasm_String (FILE* out, int cell)
    {
    assert (out);

    Commands_Name_Code commands;
    int command, reg_1, reg_2, im_mod, im_const, im_adr = 0;

    CUT_CELL(cell, command, reg_1, reg_2, im_mod, im_const, im_adr)

    string str = commands.code_str[command];
    fprintf (out, "\t%s", str.c_str());

    if (command == HALT || command == SYSCALL || command == ADDI || command == SUBI || command == MULI || command == DIVI ||
        command == LC || command == SHLI || command == SHRI || command == ANDI || command == ORI || command == XORI || command == NOT ||
        command == PUSH || command == POP || command == RET || command == CMPI || command == LOAD || command == STORE || command == LOAD2 ||
        command == STORE2)
        {
        fprintf (out, " r%d %d\n", reg_1, im_const);
        return true;
        }
    else if (command == JNE || command == JEQ || command == JLE || command == JL || command == JGE || command == JG || command == CALLI
            || command == END || command == JMP)
        {
        fprintf (out, " label_%d\n", im_adr);
        return true;
        }
    else
        {
        fprintf (out, " r%d r%d %d\n", reg_1, reg_2, im_mod);
        return true;
        }

    return true;
    }

bool Create_Labels (int num_strings, int* cells, int** labels)
    {
    assert (cells);
    assert (labels);

    for (int i = 0; i < num_strings; i++)
        {
        int command = cells[i] >> (CELL_SIZE - COMMAND_SIZE);
        int im_const = cells[i] & 0xFFFFF;
        if (command == JNE || command == JEQ || command == JLE || command == JL || command == JGE || command == JG || command == CALLI
            || command == END || command == JMP)
            {
            if (im_const >= num_strings)
                return false;
            else
                (*labels)[im_const] = 1;
            }
        }

    return true;
    }


