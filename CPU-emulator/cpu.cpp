#include <iostream>
#include <stdio.h>
#include <assert.h>
#include <map>
#include "asm.h"
#include <cmath>
#include <cstring>
#include "cpu.h"

using namespace std;

CPU::CPU ():
    memory_ (new int[MEM_SIZE]),
    reg_ ()
    {
    reg_.Stack_Pointer = MEM_SIZE;
    reg_.Program_Counter = 0;
    }

bool CPU::Read_Program (int* ready_code)
    {
    cout << "\n===Function \"CPU::Read_Program\" is active.===\n\n";

    assert (ready_code);
    FILE* empty_dump = fopen ("dump.txt", "w");
    fclose (empty_dump);

    memcpy (memory_, ready_code + 2, ready_code[0]*sizeof(ready_code[0]));

    reg_.Program_Counter = ready_code[1];

    delete (ready_code);
    IS_OK()

    cout << "\n===Function \"CPU::Read_Program\" successfully executed.===\n\n";

    return true;
    }

bool CPU::Complete_Program ()
    {
    cout << "\n===Function \"CPU::Complit_Program\" is active.===\n";
    cout << "\n--------------------------------------------------------\n";

    IS_OK()
    int res = OK;

    while (res == OK)
        {
        try
            {
            res = Complete_String ();
            }
        catch (const char* str)
            {
            cout << "error on line" << reg_.Program_Counter << ' ' << str;
            }

        reg_.Program_Counter++;
        }

    if (res == FIN)
        {
        IS_OK()
        cout << "\n\n--------------------------------------------------------\n";
        cout << "\n===Function \"CPU::Complit_Program\" successfully executed.===\n\n";

        return true;
        }
    else if (res == FAIL)
        {
        IS_OK()

        return false;
        }

    IS_OK()

    cout << "\n\n--------------------------------------------------------\n";
    cout << "\n===Function \"CPU::Complit_Program\" successfully executed.===\n\n";

    return true;
    }

int CPU::Complete_String ()
    {
    IS_OK()

    int cell = memory_[reg_.Program_Counter];
    int command, reg_1, reg_2, im_mod, im_const,im_adr = 0;

    int *reg_to = NULL;
    int *reg_from = NULL;

    CUT_CELL(cell, command, reg_1, reg_2, im_mod, im_const, im_adr)

    DEF_REGS(reg_1, reg_2, reg_to, reg_from)

    DoubleToInt conv;
    double double_view1, double_view2;

    if (command == ADDD || command == SUBD || command == MULD || command == DIVD
                        || command == ITOD || command == DTOI)
        {
        conv.int_view[0] = *reg_to;
        conv.int_view[1] = *(reg_to + 1);
        double double_view1 = conv.doub_view;

        conv.int_view[0] = *reg_from;
        conv.int_view[1] = *(reg_from + 1);
        double double_view2 = conv.doub_view;
        }

    switch (command)
        {
        case HALT:
            return FIN;
        case SYSCALL:
            {
            int res = FAIL;
            if ((res = Syscall (reg_1, im_const)) == FAIL)
                return FAIL;
            else if (res == RETURN)
                return FIN;
            break;
            }
        case ADD:
            *reg_to +=(*reg_from + im_mod) & 0xFFFFFFFF;
            break;
        case ADDI:
            *reg_to += im_const;
            break;
        case SUB:
            *reg_to += (-*reg_from + im_mod) & 0xFFFFFFFF;
            break;
        case SUBI:
            *reg_to -= im_const;
            break;
        case MUL:
            {
            long long res = (long long) ((long long) (*reg_to) * (long long) (*reg_from));
            *(reg_to + 1) = (int) (res >> 31);
            *reg_to = (int) (res & 0x7FFFFFFF);
            break;
            }
        case MULI:
            {
            long long res = (long long) (*reg_to) * (long long) (im_const);
            *(reg_to + 1) = (int) (res >> 31);
            *reg_to = (int) (res & 0x7FFFFFFF);
            break;
            }
        case DIV:
            {
            LongLong_Int convlong;
            convlong.int_view[0] = *reg_to << 1;
            convlong.int_view[1] = *(reg_to + 1);

            if (*reg_from == 0)
                throw "division on zero, command 'div'\n";

            convlong.long_view = int (((convlong.long_view >> 1) / *reg_from) << 1);
            *reg_to = convlong.int_view[0] >> 1;
            *(reg_to + 1) = convlong.int_view[1];
            break;
            }
        case DIVI:
            {
            LongLong_Int convlong;
            convlong.int_view[0] = *reg_to << 1;
            convlong.int_view[1] = *(reg_to + 1);

            if (im_const == 0)
                throw "division on zero, command 'div'\n";

            convlong.long_view = int (((convlong.long_view >> 1) / im_const) << 1);
            *reg_to = convlong.int_view[0] >> 1;
            *(reg_to + 1) = convlong.int_view[1];
            break;
            }
        case LC:
            *reg_to = im_const;
            break;
        case SHL:
            *reg_to = *reg_to >> *reg_from;
            break;
        case SHLI:
            *reg_to = *reg_to >> im_const;
            break;
        case SHR:
            if (*reg_from < 32)
                *reg_to = *reg_to << *reg_from;
            else
                *reg_to = 0;
            break;
        case SHRI:
            if (*reg_from < 32)
                *reg_to = *reg_to << im_const;
            else
                *reg_to = 0;
            break;
        case AND:
            *reg_to = *reg_to & *reg_from;
            break;
        case ANDI:
            *reg_to = *reg_to & im_const;
            break;
        case OR:
            *reg_to = *reg_to | *reg_from;
            break;
        case ORI:
            *reg_to = *reg_to | im_const;
            break;
        case XOR:
            *reg_to = *reg_to ^ *reg_from;
            break;
        case XORI:
            *reg_to = *reg_to ^ im_const;
            break;
        case NOT:
            *reg_to = ~(*reg_to);
            break;
        case MOV:
            *reg_to = *reg_from + im_mod;
            break;
        case ADDD:
            conv.doub_view = double_view1 + double_view2;

            *reg_to = conv.int_view[0];
            *(reg_to + 1) = conv.int_view[1];
            break;
        case SUBD:
            conv.doub_view = double_view1 - double_view2;

            *reg_to = conv.int_view[0];
            *(reg_to + 1) = conv.int_view[1];
            break;
        case MULD:
            conv.doub_view = double_view1 * double_view2;

            *reg_to = conv.int_view[0];
            *(reg_to + 1) = conv.int_view[1];
            break;
        case DIVD:
            conv.doub_view = double_view1 / double_view2;

            if (double_view2 <= EPSILON)
                throw "division on zero\n";

            *reg_to = conv.int_view[0];
            *(reg_to + 1) = conv.int_view[1];
            break;
        case PUSH:
            memory_[--reg_.Stack_Pointer] = *reg_to + im_const;
            break;
        case POP:
            if (reg_.Stack_Pointer < MEM_SIZE)
                {
                *reg_to = memory_[reg_.Stack_Pointer] + im_const;
                memory_[reg_.Stack_Pointer++] = 0;
                }
            else
                throw "wrong call of function 'pop': stack is empty\n";
            break;
        case ITOD:
			conv.doub_view = (double) *reg_from;
			*reg_to = conv.int_view[0];
			*(reg_to + 1) = conv.int_view[0];
			break;
		case DTOI:
			conv.int_view[0] = *reg_from;
			conv.int_view[1] = *(reg_from + 1);
			if (conv.doub_view > (1 << 31) || conv.doub_view < (int) 0xFFFFFFFF)
				throw "can't make conversation from double to int, command dtoi\n";

			*reg_to = (int)conv.doub_view;
			break;
        case CALL:
            memory_[--reg_.Stack_Pointer] = reg_.Program_Counter;
            *reg_to = reg_.Program_Counter;
            reg_.Program_Counter = *reg_from - 1;
            break;
        case CALLI:
            memory_[--reg_.Stack_Pointer] = reg_.Program_Counter;
            reg_.Program_Counter = im_const - 1;
            break;
        case RET:
            reg_.Program_Counter = memory_[reg_.Stack_Pointer];
            memory_[reg_.Stack_Pointer++] = 0;
            for (int i = 0; i < im_const; i++)
                 memory_[reg_.Stack_Pointer++] = 0;
            break;
        case CMP:
            reg_.flags = *reg_to - *reg_from;
            break;
        case CMPI:
            reg_.flags = *reg_to - im_const;
            break;
        case CMPD:
            {
            double res = double_view1 - double_view2;
            reg_.flags = (int) res;
            break;
            }
        case JMP:
            reg_.Program_Counter = im_adr - 1;
            break;
        case JNE:
            if (reg_.flags != EQUAL)
                reg_.Program_Counter = im_adr - 1;
            break;
        case JEQ:
            if (reg_.flags == EQUAL)
                reg_.Program_Counter = im_adr - 1;
            break;
        case JLE:
            if (reg_.flags == EQUAL || reg_.flags < 0)
                reg_.Program_Counter = im_adr - 1;
            break;
        case JL:
            if (reg_.flags < 0)
                reg_.Program_Counter = im_adr - 1;
            break;
        case JGE:
            if (reg_.flags == EQUAL || reg_.flags > 0)
                reg_.Program_Counter = im_adr - 1;
            break;
        case JG:
            if (reg_.flags > 0)
                reg_.Program_Counter = im_adr - 1;
            break;
        case LOAD:
            *reg_to = memory_[im_adr];
            break;
        case STORE:
            memory_[im_adr] = *reg_to;
            break;
        case LOAD2:
            *reg_to = memory_[im_const];
            *(reg_to + 1) = memory_[im_const + 1];
            break;
        case STORE2:
            memory_[im_const] = *reg_to;
            memory_[im_const + 1] = *(reg_to + 1);
            break;
        case LOADR:
            *reg_to = memory_[im_mod + *reg_from];
            break;
        case STORER:
            memory_[im_const + *reg_from] = *reg_to;
            break;
        case LOADR2:
            *reg_to = memory_[im_const + *reg_from];
            *(reg_to + 1) = memory_[im_const + 1 + *reg_from];
            break;
        case STORER2:
            memory_[im_const + *reg_from] = *reg_to;
            memory_[im_const + 1 + *reg_from] = *(reg_to + 1);
            break;
        case END:
            break;
        default:
            IS_OK()
            return FAIL;
            break;
        }
    IS_OK()

    return OK;
    }

int CPU::Syscall (int reg, int code)
    {
    IS_OK()
    switch (code)
        {
        case EXIT:
            return RETURN;
            break;
        case SCANINT:
            {
            int num = 0;
            cout << '\n';
            cin >> num;
            reg_.Register[reg] = num;
            break;
            }
        case SCANDOUBLE:
            {
            double num = 0.0;
            cin >> num;
            DoubleToInt conv;
            conv.doub_view = num;

            reg_.Register[reg] = conv.int_view[0];
            reg_.Register[reg + 1] = conv.int_view[1];
            break;
            }
        case PRINTINT:
            cout << reg_.Register[reg] << " ";
            break;
        case PRINTDOUBLE:
            {
            DoubleToInt conv;
            conv.int_view[0] = reg_.Register[reg];
            conv.int_view[1] = reg_.Register[reg + 1];
            double double_view = conv.doub_view;

            cout << double_view << '\n';
            break;
            }
        case GETCHAR:
            {
            char c;
            cout << '\n';
            cin >> c;
            reg_.Register[reg] = c;
            break;
            }
        case PUTCHAR:
            cout << (char) reg_.Register[reg] << " ";
            break;
        default:
            IS_OK()
            return FAIL;
            break;
        }
    IS_OK()

    return OK;
    }

bool CPU::Dump ()
    {
    assert (this);
    FILE* dump = fopen ("dump.txt", "a");

    fprintf (dump, "===HI, IT'S DUMP, GLAD TO SEE YOU!===\n");
    fprintf (dump, "this = %p\n", this);
    fprintf (dump, "memory_ = %p\n", memory_);

    for (int i = 0; i < 13; i++)
        fprintf (dump, "\treg_.Register[%d] = %d\n", i, reg_.Register[i]);
    fprintf (dump, "reg_.Program_Counter = %d\n", reg_.Program_Counter);
    fprintf (dump, "reg_.Stack_Pointer = %d\n", reg_.Stack_Pointer);
    fprintf (dump, "reg_.flags = %d\n", reg_.flags);
    fprintf (dump, "===DUMP IS OVER, YOU'RE WELCOME===\n");
    fclose (dump);

    return true;
    }

int CPU::Ok ()
    {
    int num_fails = 0;

    if (memory_ == NULL)
        num_fails++;
    if (reg_.Stack_Pointer > MEM_SIZE)
        num_fails++;
    if (reg_.Program_Counter >= (signed) reg_.Stack_Pointer)
        num_fails++;

    return num_fails;
    }

bool CPU::Present_File (FILE* out)
    {
    cout << "\n===Function \"CPU::Present_File\" is active.===\n\n";

    assert (out);
    IS_OK()
    fprintf (out, "ThisIsFUPM2Exec\n");

    int fin = 0;
    int size_of_code = 0;

    while (fin < 3)
        if (memory_[size_of_code++] == 0)
            fin ++;
        else if (fin != 0)
            fin = 0;

    size_of_code -= 3;
    fprintf (out, "%d\n", size_of_code);

    for (int i = 0; i < size_of_code; i++)
        fprintf (out, "\n%d", memory_[i]);

    IS_OK()

    cout << "\n===Function \"CPU::Present_File\" successfully executed.===\n\n";

    return true;
    }


















