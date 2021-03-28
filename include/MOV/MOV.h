#pragma once

#include <CPU.h>

typedef struct{
    unsigned d : 1; //d for "To reg" or 
    unsigned w : 1; //word or byte instruction
    unsigned mod: 2; //mod field
    unsigned reg : 3; //reg field
    unsigned rm : 3; // r/m field

}mov_register_or_memory_to_or_from_register_fields;

typedef struct 
{
    unsigned w : 1;//word or byte instruction
    unsigned mod : 2;//mod field
    unsigned rm : 3;// r/m field
     
}inmediate_to_register_or_memory_fields;

typedef struct 
{
    unsigned w : 1;//word or byte instruction
    unsigned reg : 3;//reg field
    
}inmediate_to_register_fields;



class MOV
{
private:
    /* data */

public:
    MOV(/* args */);
    void execute_mov_register_or_memory_to_or_from_register(uint8_t opcode);//Register/Memory to/from Register
    void execute_inmediate_to_register_or_memory(uint8_t opcode); //Immediate to Register/Memory
    void execute_inmediate_to_register(uint8_t opcode); //Immediate to Register
    void execute_memory_to_accumulator(uint8_t opcode);////Memory to Accumulator
    void execute_accumulator_to_memory(uint8_t opcode); ////Accumulator to Memory
    void write_or_read_ram(uint16_t disp, mov_register_or_memory_to_or_from_register_fields *fields);
    ~MOV();
};

