#pragma once

#include <CPU.h>

//push Segment Register

class PUSH_2_OPCODE
{

public:
    PUSH_2_OPCODE();
    void execute(uint8_t opcode);
    ~PUSH_2_OPCODE();
};

