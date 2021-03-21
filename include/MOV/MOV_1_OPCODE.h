#pragma once

#include <CPU.h>

class MOV_1_OPCODE
{
private:
    CPU *cpu;

public:
    MOV_1_OPCODE(CPU *cpu);
    void execute(uint8_t opcode);
    ~MOV_1_OPCODE();
};

