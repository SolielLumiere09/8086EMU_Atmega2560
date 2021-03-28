#pragma once 

#include <CPU.h>


typedef struct 
{
    unsigned w : 1;
    unsigned mod : 2;
    unsigned reg : 3;
    unsigned rm : 3;

}xchg_register_or_memory_with_register_fields;


class XCHG
{
private:
    /* data */
public:
    XCHG(/* args */);
    void execute_register_or_memory_with_register(uint8_t opcode);//Register/Memory with Register
    void execute_register_with_accumulator(uint8_t opcode);//Register with Accumulator
    ~XCHG();
};

