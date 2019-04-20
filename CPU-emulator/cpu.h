#ifndef CPU_H_INCLUDED
#define CPU_H_INCLUDED
#include <map>
#include "asm.h"
#include <cmath>
#include <cstring>

using namespace std;

#define IS_OK()                             \
    {                                       \
    }

#define DEF_REGS(reg_1, reg_2, reg_to, reg_from)    \
    {                                               \
    if (reg_1 == 14)                                \
        reg_to = (int*)&reg_.Stack_Pointer;         \
    else if (reg_1 == 13)                           \
        reg_to = (int*)&reg_.Program_Counter;       \
    else if (reg_1 == 15)                           \
        reg_to = (int*)&reg_.Frame_Pointer;         \
    else if (reg_1 < 13)                            \
        reg_to = &reg_.Register[reg_1];             \
    if (reg_2 == 14)                                \
        reg_from = (int*)&reg_.Stack_Pointer;       \
    else if (reg_2 == 13)                           \
        reg_from = (int*)&reg_.Program_Counter;     \
    else if (reg_2 == 15)                           \
        reg_from = (int*)&reg_.Frame_Pointer;         \
    else if (reg_2 < 13)                            \
        reg_from = &reg_.Register[reg_2];           \
    }

#define CUT_CELL(cell, command, reg_1, reg_2, im_mod, im_const, im_adr) \
    {                                                           \
    command = cell >> (CELL_SIZE - COMMAND_SIZE);               \
    reg_1 = (cell >> (CELL_SIZE - COMMAND_SIZE - NUM_REG_SIZE)) & 0xF;  \
    reg_2 = (cell >> (CELL_SIZE - COMMAND_SIZE - 2*NUM_REG_SIZE))& 0xF; \
    im_adr = (cell & 0xFFFFF); \
    im_mod = ((cell & 0xFFFF) << (CELL_SIZE - IM_MOD_SIZE)) >> (CELL_SIZE - IM_MOD_SIZE); \
    im_const = ((cell & 0xFFFFF) << (CELL_SIZE - IM_CONST_SIZE)) >> (CELL_SIZE - IM_CONST_SIZE);\
    }

const size_t MEM_SIZE = 1 << 20;
const int OK = 0;
const int FIN = 2;
const int RETURN = 3;
const int EQUAL = 0;
const int NONE = -1;
const double EPSILON = 0.000001;

union DoubleToInt
                {
                double doub_view;
                int int_view[2];
                };

union LongLong_Int
                {
                long long long_view;
                int int_view[2];
                };

typedef struct Reg
    {
    int Register[13] = {0};
    unsigned int Frame_Pointer = 0;
    unsigned int Stack_Pointer = 0;
    int Program_Counter = 0;
    int flags = 0;
    }Reg;

class CPU
    {
    public:
    CPU ();
    bool Complete_Program ();
    int Complete_String ();
    bool Read_Program (int* ready_code);
    int Syscall (int reg, int im_const);
    int Ok ();
    bool Dump ();
    bool Present_File (FILE* out);
    bool Present_String_Code (FILE* out, int str_num);

    private:
    Reg reg_;
    int* memory_;
    };

enum syscodes   {EXIT = 0, OPEN = 1, READ = 2, WRITE = 3, CLOSE= 4, ALLOC = 5,
                FREE = 6, SCANINT = 100, SCANDOUBLE = 101, PRINTINT = 102, PRINTDOUBLE = 103, GETCHAR = 104, PUTCHAR = 105};

#endif // CPU_H_INCLUDED
