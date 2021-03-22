#pragma once


#include<CPU.h>

class MOV_3_OPCODE
{
private:
    CPU *cpu;

public:
    MOV_3_OPCODE(CPU *cpu);
    void execute(uint8_t opcde);
    ~MOV_3_OPCODE();
};

