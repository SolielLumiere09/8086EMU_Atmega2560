#pragma once

#include<CPU.h>

typedef struct{
    unsigned mod : 2;
    unsigned rm : 3;

}push_register_or_memory_fields;

class PUSH
{
private:
    /* data */
public:
    PUSH(/* args */);
    void execute_register_or_memory();//push Register or Memory
    void execute_register(uint8_t opcode);//push register
    void execute_segment_register(uint8_t opcode);//push segment  
    ~PUSH();
};

