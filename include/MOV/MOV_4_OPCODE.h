#pragma once


#include<CPU.h>

//Accumulator to Memory

class MOV_4_OPCODE
{

public:
    MOV_4_OPCODE();
    void execute(uint8_t opcode);
    ~MOV_4_OPCODE();
};
