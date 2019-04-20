#include <iostream>
#include "asm.h"
#include "cpu.h"
#include <stdio.h>
#include <ctime>
#include "disasm.h"

int StartProcessor (FILE* read);

int StartProcessor (FILE* read)
    {
    double time_for_function = 0;
    int* ready_code = Read_Asm_File (read);
    time_for_function = clock ();
    cout << '\n' << (double)time_for_function / CLOCKS_PER_SEC << '\n';
    CPU our_processor;
    our_processor.Read_Program (ready_code);
    cout << '\n' << (double) (clock () - time_for_function) / CLOCKS_PER_SEC << '\n';
    time_for_function = clock ();
    our_processor.Complete_Program ();
    cout << '\n' << (double) (clock () - time_for_function) / CLOCKS_PER_SEC << '\n';
    time_for_function = clock ();
    our_processor.Dump ();
    FILE* FUPMOS = fopen ("C:\\Users\\Eugenii\\Documents\\cb\\2 semak\\Fupm2\\FUPMOS.txt", "w");
    our_processor.Present_File (FUPMOS);
    cout << '\n' << (double) (clock () - time_for_function) / CLOCKS_PER_SEC << '\n';
    time_for_function = clock ();
    fclose (FUPMOS);
    FILE* FUPMOS_2 = fopen ("C:\\Users\\Eugenii\\Documents\\cb\\2 semak\\Fupm2\\FUPMOS.txt", "r");
    FILE* disasm = fopen ("C:\\Users\\Eugenii\\Documents\\cb\\2 semak\\Fupm2\\disasm.txt", "w");
    Disasm_Program (FUPMOS_2, disasm);
    cout << '\n' << (double) (clock () - time_for_function) / CLOCKS_PER_SEC << '\n';
    fclose (FUPMOS_2);
    fclose (disasm);

    return OK;
    }

int main (int argc, char* argv[])
    {
    FILE* read = NULL;
    if (argc > 1)
        {
        cout << '.' << argv[1] << '.' << "\n";
        read = fopen (argv[1], "r");
        }
    else
        read = fopen ("tests\\lucky ticket.txt", "r");

    StartProcessor (read);

    fclose (read);

    return 0;
    }
