#pragma once

#include <CPU.h>

class MOV_0_OPCODE ////Register/Memory to/from Register
{
    private:
        CPU *cpu;


    public:
        MOV_0_OPCODE(CPU *cpu);
        void execute(uint8_t opcode);
        
        ~MOV_0_OPCODE();
};
