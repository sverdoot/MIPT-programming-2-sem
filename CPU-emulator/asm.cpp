#include <iostream>
#include <stdio.h>
#include <assert.h>
#include <algorithm>
#include "asm.h"

using namespace std;

Commands_Name_Code::Commands_Name_Code ()
    {
    }

Commands_Name_Code::~Commands_Name_Code ()
    {
    }

int* Read_Asm_File (FILE* file_asm) {
    cout << "\n===Function \"Read_Asm_File\" is active.===\n\n";

    //assert (file_asm);

    fseek (file_asm, 0, SEEK_END);
    size_t fsize = ftell (file_asm);
    fseek (file_asm, 0, SEEK_SET);

    char* asm_text = new char[fsize + 1];
    fread (asm_text, sizeof (asm_text[0]), fsize, file_asm);
    printf (".%s.", asm_text);

    asm_text[fsize] = '\0';

    map <string, int> Labels = {};
    int num_strings = Read_Labels (asm_text, Labels);

    int* ready_code = new int[num_strings + 2];
    ready_code[0] = num_strings;
    for (int i = 1; i < num_strings + 2; i++)
        ready_code[i] = 0;

    int* copy = ready_code + 2;
    while (Read_Asm_String (asm_text, copy, Labels));
    int last_command = 0;
    if ((last_command = *(copy - 1) >> (CELL_SIZE - COMMAND_SIZE)) == END)
        ready_code[1] = *(copy - 1) & 0xFFFFF;

    delete []asm_text;

    cout << "\n===Function \"Read_Asm_File\" successfully executed.===\n\n";

    return ready_code;
    }

int Read_Labels (char* asm_text, map <string, int> &Labels)
    {
    assert (asm_text);

    char* ptr_copy = asm_text;
    int i = 0;
    string label = "";
    int str = 0;

    while (*asm_text != '\0')
        {
        bool comment = false;
        bool space = true;
        bool code = false;

        while ((*asm_text != ':') && (*asm_text != '\0'))
            {
            if (!comment && isalpha (*asm_text))
                code = true;

            if (*asm_text == ';')
                {
                asm_text++;
                comment = true;
                }

            if (*asm_text != ' ' && *asm_text != '\t' && *asm_text != '\0' && *asm_text != '\n')
                space = false;

            if (*asm_text == '\n')
                {
                if (!space && code)
                    str++;
                comment = false;
                space = true;
                code = false;
                }

            asm_text++;
            }
        if (*asm_text == '\0')
            return str + 2;

        asm_text--;

        while ((isalpha (*asm_text) || isdigit (*asm_text) || *asm_text == '_') &&  asm_text >= ptr_copy)
            {
            label += * (asm_text--);
            i++;
            }
        if (*asm_text == '\n' || asm_text == ptr_copy - 1)
            {
            reverse (label.begin(), label.end());
            Labels[label] = str;
            //cout << label << ' ' << str << '\n';
            }

        label = "";
        asm_text += i + 2;
        while (!isalpha (*asm_text))
            {
            if (*asm_text == ';')
                break;
            asm_text++;
            }
        if (*asm_text == ';')
            {
            while (*asm_text != '\n')
                asm_text++;
            asm_text++;
            }

        i = 0;
        }

    return str + 2;
    }

bool Read_Asm_String (char* &asm_text, int* &ready_code, map <string, int> &Labels)
    {
    assert (asm_text);
    //assert (*asm_text);
    assert (ready_code);

    while (!isalpha(*asm_text) && *asm_text != '\0' && *asm_text != ';')
        asm_text++;

    if (*asm_text == ';')
        {
        while ((*asm_text != '\n') && (*asm_text != '\0'))
            (asm_text)++;

        if (*asm_text == '\0')
        return false;

        while ((*asm_text < 'A' ||*asm_text > 'z') && *asm_text != '\0')
            asm_text ++;
        }

    if (*asm_text == '\0')
        return false;

    int reg_to = 0;
    int reg_from = -1;
    int num = 0;
    string label = "";

    int code = Read_Command (asm_text);

    while (!isalpha (*asm_text) && !isdigit (*asm_text))
        asm_text ++;

    if (*asm_text == 'r' && isdigit (*(asm_text + 1)))
        {
        asm_text ++;
        reg_to = Get_Int (asm_text);
        asm_text ++;
        }

    if (*asm_text == 'r' && isdigit (*(asm_text + 1)))
        {
        asm_text ++;
        reg_from = Get_Int (asm_text);
        asm_text ++;
        num = Get_Int (asm_text);
        }
    else if (isdigit (*asm_text))
        num = Get_Int (asm_text);
    else if (isalpha (*asm_text))
        {
        while (isalpha (*asm_text) || isdigit (*asm_text) || *asm_text == '_')
            label += *(asm_text)++;
        num = Labels[label];
        label = "";
        }
    else if (*asm_text == '-')
        {
        (asm_text)++;
        num = -Get_Int (asm_text);
        }
    else
        return false;

    int cell = code << (CELL_SIZE - COMMAND_SIZE);
    cell += reg_to << (CELL_SIZE - COMMAND_SIZE - NUM_REG_SIZE);
    if (reg_from != -1)
        {
        cell += reg_from << (CELL_SIZE - COMMAND_SIZE - 2*NUM_REG_SIZE);
        cell += num & 0xFFFF;
        }
    else
        cell += num & 0xFFFFF;

    *(ready_code ++) = cell;

    return true;
    }

int Read_Command (char* &asm_text)
    {
    assert (asm_text);

    Commands_Name_Code commands_;

    string command = "";
    int code = -1;

    while (isalpha (*asm_text))
        {
        command += *asm_text;
        (asm_text) ++;
        }

    while (isdigit (*asm_text) || *asm_text == '_')
        (asm_text) ++;

    if ((*asm_text) == ':')
        {
        while (!isalpha (*asm_text))
            {
            if ((*asm_text) == ';')
                break;
            asm_text ++;
            }

        if ((*asm_text) == ';')
            {
            while ((*asm_text) != '\n')
                asm_text ++;
            while (!isalpha (*asm_text))
                asm_text ++;
            }

        code = Read_Command (asm_text);
        return code;
        }

    code = commands_.str_code [command];

    return code;
    }

int Get_Int (char* &asm_text)
    {
    assert (asm_text);
    int num = 0;

    while (isdigit (*asm_text))
        {
        num = num * 10 + *asm_text - '0';
        asm_text ++;
        }

    return num;
    }







