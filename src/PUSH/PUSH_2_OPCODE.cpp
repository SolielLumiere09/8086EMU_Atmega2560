#include <PUSH/PUSH_2_OPCODE.h>

PUSH_2_OPCODE::PUSH_2_OPCODE(/* args */)
{
}
void PUSH_2_OPCODE::execute(uint8_t opcode){
    #define REG_L 3
    #define REG_H 4

    uint8_t reg = CPU::cpu->get_bits(REG_L, REG_H, opcode); // get reg field 
    
    CPU::cpu->registers[SP_REG].data -= 2; //decrement stack pointer
    CPU::cpu->write_to_ram((uint16_t*)CPU::cpu->registers[SP_REG].data - 1, CPU::cpu->segment_registers[reg].data_HL[HIGH]); //write high part segment to stack
    CPU::cpu->write_to_ram((uint16_t*)CPU::cpu->registers[SP_REG].data - 2, CPU::cpu->segment_registers[reg].data_HL[LOW]); //write low part segment to stack
    
    

    #undef REG_L
    #undef REG_H
}
PUSH_2_OPCODE::~PUSH_2_OPCODE()
{
}
