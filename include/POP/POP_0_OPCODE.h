#pragma once

#include <CPU.h>


typedef struct{
    unsigned mod : 2;
    unsigned rm : 3;
}POP_0_FIELDS;

class POP_0_OPCODE
{

public:
    POP_0_OPCODE(/* args */);
    void execute();
    ~POP_0_OPCODE();
};


