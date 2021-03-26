#include <PUSH/PUSH_1_OPCODE.h>



PUSH_1_OPCODE::PUSH_1_OPCODE()
{
    
}
void PUSH_1_OPCODE::execute(uint8_t opcode){
    #define REG_L 0
    #define REG_H 2
    
    uint8_t reg = CPU::cpu->get_bits(REG_L, REG_H, opcode); //get reg field
 
    CPU::cpu->write_to_ram((uint8_t*)CPU::cpu->registers[SP_REG].data - 1, CPU::cpu->registers[reg].data_HL[HIGH]); //write high part of reg
    CPU::cpu->write_to_ram((uint8_t*)CPU::cpu->registers[SP_REG].data - 2, CPU::cpu->registers[reg].data_HL[LOW]); //write low part of reg
    

    CPU::cpu->registers[SP_REG].data -= 2; //decrement SP
    
    #undef REG_L
    #undef REG_H
}
PUSH_1_OPCODE::~PUSH_1_OPCODE()
{
}
