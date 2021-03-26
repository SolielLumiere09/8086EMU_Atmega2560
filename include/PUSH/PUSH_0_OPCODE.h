#pragma once


#include <CPU.h>

typedef struct{
    unsigned mod : 2;
    unsigned rm : 3;

}PUSH_0_FIELDS;

class PUSH_0_OPCODE
{
public:
    PUSH_0_OPCODE();
    void execute();
    void write_to_stack(uint8_t rm, uint16_t disp);
    ~PUSH_0_OPCODE();
};

