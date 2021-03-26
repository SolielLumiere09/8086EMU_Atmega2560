#pragma once

#include<CPU.h>


typedef struct 
{
    unsigned w : 1;
    unsigned reg : 3;
    
}MOV_2_FIELDS;


class MOV_2_OPCODE
{

public:
    MOV_2_OPCODE();
    void execute(uint8_t opcode);
    ~MOV_2_OPCODE();
};
