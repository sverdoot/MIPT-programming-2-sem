#ifndef COMMANDS_H_INCLUDED
#define COMMANDS_H_INCLUDED
#include <map>
#include <string>

using namespace std;

enum code
        {
        HALT = 0, SYSCALL = 1, ADD = 2, ADDI = 3, SUB = 4, SUBI = 5, MUL = 6, MULI = 7, DIV = 8, DIVI = 9, LC = 12,
        SHL = 13, SHLI = 14, SHR = 15,SHRI = 16, AND = 17, ANDI = 18, OR = 19, ORI = 20, XOR = 21, XORI = 22,
        NOT = 23, MOV = 24, ADDD = 32, SUBD = 33, MULD = 34, DIVD = 35,ITOD = 36, DTOI = 37,PUSH = 38, POP = 39,
        CALL = 40, CALLI = 41, RET = 42, CMP = 43, CMPI = 44, CMPD = 45, JMP = 46, JNE = 47,JEQ = 48, JLE = 49,
        JL = 50, JGE = 51, JG = 52, LOAD = 64,STORE = 65, LOAD2 = 66, STORE2 = 67, LOADR = 68, LOADR2 = 69,
        STORER = 70, STORER2 = 71, END = 72, WORD = 73
        };

class Commands_Name_Code
    {
    public:

    Commands_Name_Code ();
    ~Commands_Name_Code ();

    map <string, int> str_code =
        {
        {"halt", HALT}, {"syscall", SYSCALL}, {"add", ADD}, {"addi", ADDI}, {"sub", SUB}, {"subi", SUBI}, {"mul", MUL}, {"muli", MULI},
        {"div", DIV}, {"divi", DIVI}, {"lc", LC}, {"shl", SHL}, {"shli", SHLI}, {"shr", SHR}, {"shri", SHRI}, {"and", AND}, {"andi", ANDI},
        {"or", OR}, {"ori", ORI}, {"xor", XOR}, {"xori", XORI}, {"not", NOT}, {"mov", MOV}, {"addd", ADDD}, {"sub", SUBD }, {"muld", MULD},
        {"divd", DIVD}, {"itod", ITOD}, {"dtoi", DTOI}, {"push", PUSH}, {"pop", POP}, {"call", CALL}, {"calli", CALLI}, {"ret", RET},
        {"cmp", CMP}, {"cmpi", CMPI}, {"cmpd", CMPD}, {"jmp", JMP}, {"jne", JNE}, {"jeq", JEQ}, {"jle", JLE}, {"jl", JL}, {"jge", JGE}, {"jg", JG},
        {"load", LOAD}, {"store", STORE}, {"load2", LOAD2}, {"store2", STORE2}, {"loadr", LOADR}, {"loadr2", LOADR2}, {"storer", STORER},
        {"storer2", STORER2}, {"end", END}, {"word", WORD}
        };

    map <int, string> code_str =
        {
        {HALT, "halt"}, {SYSCALL, "syscall"}, {ADD, "add"}, {ADDI, "addi"}, {SUB, "sub"}, {SUBI, "subi"}, {MUL, "mul"}, {MULI, "muli"},
        {DIV, "div"}, {DIVI, "divi"}, {LC, "lc"}, {SHL, "shl"}, {SHLI, "shli"}, {SHR, "shr"}, {SHRI, "shri"}, {AND, "and"}, {ANDI, "andi"},
        {OR, "or"}, {ORI, "ori"}, {XOR, "xor"}, {XORI, "xori"}, {NOT, "not"}, {MOV, "mov"}, {ADDD, "addd"}, {SUBD, "subd"}, {MULD, "muld"},
        {DIVD, "divd"}, {ITOD, "itod"}, {DTOI, "dtoi"}, {PUSH, "push"}, {POP, "pop"}, {CALL, "call"}, {CALLI, "calli"}, {RET, "ret"},
        {CMP, "cmp"}, {CMPI, "cmpi"}, {JMP, "jmp"}, {JNE, "jne"}, {JEQ, "jeq"}, {JLE, "jle"}, {JL, "jl"}, {JGE, "jge"}, {JG, "jg"},
        {LOAD, "load"}, {STORE, "store"}, {LOAD2, "load2"}, {STORE2, "store2"}, {LOADR, "loadr"}, {LOADR2, "loadr2"}, {STORER, "storer"},
        {STORER2, "storer2"}, {END, "end"}, {WORD, "word"}
        };
    };

#endif // COMMANDS_H_INCLUDED
