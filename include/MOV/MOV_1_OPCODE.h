#pragma once

#include <CPU.h>


typedef struct 
{
    unsigned w : 1;
    unsigned mod : 2;
    unsigned rm : 3;
     
}MOV_1_FIELDS;


class MOV_1_OPCODE
{
private:
    CPU *cpu;

public:
    MOV_1_OPCODE(CPU *cpu);
    void execute(uint8_t opcode);
    ~MOV_1_OPCODE();
};

