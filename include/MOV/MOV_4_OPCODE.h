#pragma once


#include<CPU.h>

//Accumulator to Memory

class MOV_4_OPCODE
{
private:
    CPU *cpu;

public:
    MOV_4_OPCODE(CPU *cpu);
    void execute(uint8_t opcode);
    ~MOV_4_OPCODE();
};
