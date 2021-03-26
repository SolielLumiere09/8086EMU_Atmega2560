#pragma once

#include <CPU.h>

typedef struct{
    unsigned d : 1; //d for "To reg" or 
    unsigned w : 1; //word or byte instruction
    unsigned mod: 2; //mod field
    unsigned reg : 3; //reg field
    unsigned rm : 3; // r/m field

}MOV_0_FIELDS;

class MOV_0_OPCODE ////Register/Memory to/from Register
{
    public:
        MOV_0_OPCODE();
        void execute(uint8_t opcode);
        void write_or_read_ram(uint16_t disp, MOV_0_FIELDS *fields);
        ~MOV_0_OPCODE();
};
