#include <POP/POP.h>

POP::POP(/* args */)
{
}

void POP::execute_pop_register_or_memory(uint8_t opcode){
     #define MOD_L 6
    #define MOD_H 7
    #define RM_L 0
    #define RM_H 2

    uint8_t opcode_fields = CPU::cpu->fetch(); //get fields 
    pop_register_or_memory_fields fields;

    fields.mod = CPU::cpu->get_bits(MOD_L, MOD_H, opcode_fields); // get mod field
    fields.rm = CPU::cpu->get_bits(RM_L, RM_H, opcode_fields);// get rm field

    if(fields.mod == 0b00 && fields.rm == 0b110){
        //except if mod = 00 and r/m = 110 then EA = disp-high;disp-low
        uint16_t disp = CPU::cpu->get_word_disp(SS_REG); //get disp
        uint16_t data = CPU::cpu->pop_from_stack(); //get data from stack

        CPU::cpu->write_to_ram((uint16_t*)disp, data); //write to memory

        
    }
    else if(fields.mod == 0b00){
         //if mod = 00 then DISP = 0*,disp-low and disp-high are absent
         uint16_t data = CPU::cpu->pop_from_stack(); //get data from stack
         CPU::cpu->write_to_ram((uint16_t*)CPU::cpu->get_effective_address(fields.rm, 0), data);
    }
    else if(fields.mod == 0b01){
        //if mode01 then DISP = disp-low sign-extended to 16 bits,disp-high is absent
        int16_t disp = CPU::cpu->get_word_signed_disp(); //get total disp
        uint16_t data = CPU::cpu->pop_from_stack();//get data

        CPU::cpu->write_to_ram((uint16_t*)CPU::cpu->get_effective_address(fields.rm, disp), data);//write to ram
      
    }
    else if(fields.mod == 0b10){
        //if mod = 10 then DISP = disp-high;disp-low
        uint16_t disp = CPU::cpu->get_word_disp(); //get disp 
        uint16_t data = CPU::cpu->pop_from_stack(); //get top of stack

        CPU::cpu->write_to_ram((uint16_t*)CPU::cpu->get_effective_address(fields.rm, disp), data);
       
    }
    else if(fields.mod == 0b11){
        //pop to register 
        CPU::cpu->registers[fields.rm].data = CPU::cpu->pop_from_stack();
    }

    #undef MOD_L
    #undef MOD_H
    #undef RM_L
    #undef RM_H
}

void POP::execute_pop_register(uint8_t opcode){
    #define REG_L  0
    #define REG_H 2
    
    uint8_t reg = CPU::cpu->get_bits(REG_L, REG_H, opcode); //get register
    CPU::cpu->registers[reg].data = CPU::cpu->pop_from_stack(); //pop to register

    #undef REG_L
    #undef REG_H
}
void POP::execute_pop_segment_register(uint8_t opcode){
    #define REG_L 3
    #define REG_H 4
    uint8_t reg = CPU::cpu->get_bits(REG_L, REG_H, opcode); //get segment register
    
    CPU::cpu->segment_registers[reg].data = CPU::cpu->pop_from_stack(); //pop to segment register

    #undef REG_L
    #undef REG_H

}
POP::~POP()
{
}