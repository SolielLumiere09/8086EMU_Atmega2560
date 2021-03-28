#pragma once

#include <CPU.h>

typedef struct{
    unsigned mod : 2;
    unsigned rm : 3;
}pop_register_or_memory_fields;


class POP
{
private:
    /* data */
public:
    POP(/* args */);
    void execute_pop_register_or_memory(uint8_t opcode);//pop Register/Memory
    void execute_pop_register(uint8_t opcode); //pop Register
    void execute_pop_segment_register(uint8_t opcode); //pop to register 
    ~POP();
};


