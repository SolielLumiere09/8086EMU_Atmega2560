#pragma once

#include<CPU.h>


typedef struct 
{
    unsigned w : 1;
    unsigned reg : 3;
    
}MOV_2_FIELDS;


class MOV_2_OPCODE
{
private:
    CPU *cpu;

public:
    MOV_2_OPCODE(CPU *cpu);
    void execute(uint8_t opcode);
    ~MOV_2_OPCODE();
};
