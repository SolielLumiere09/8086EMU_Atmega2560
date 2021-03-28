#include <PUSH.h>

PUSH::PUSH(/* args */)
{
}
void PUSH::execute_register_or_memory(){
    #define MOD_L 6
    #define MOD_H 7
    #define RM_L 0
    #define RM_H 2
    
    push_register_or_memory_fields fields;
    uint8_t field_opcode = CPU::cpu->fetch();

    fields.mod = CPU::cpu->get_bits(MOD_L, MOD_H, field_opcode);
    fields.rm = CPU::cpu->get_bits(RM_L, RM_H, field_opcode);
    

    if(fields.mod == 0b00 && fields.rm == 0b110){ 
        //disp-HIGH:disp-LOW
        uint16_t disp = CPU::cpu->get_word_disp(SS_REG); //get TOTAL disp 
        CPU::cpu->push_to_stack(CPU::cpu->read_from_ram((uint16_t*)disp));//push data to stack
    }
    else if(fields.mod == 0b00){
        //if mod = 00 then DISP = 0*,disp-low and disp-high are absent
        uint16_t data = CPU::cpu->read_from_ram((uint16_t*)CPU::cpu->get_effective_address(fields.rm, 0));
        CPU::cpu->push_to_stack(data);
    }
    else if(fields.mod == 0b01){
         //if mode01 then DISP = disp-low sign-extended to 16 bits,disp-high is absent
        int16_t disp = CPU::cpu->get_word_signed_disp(); //get disp
        uint16_t data = CPU::cpu->read_from_ram((uint16_t*)CPU::cpu->get_effective_address(fields.rm, (uint16_t)disp)); //read the data to push
        CPU::cpu->push_to_stack(data);
    }
    else if(fields.mod == 0b10){
         //f mod = 10 then DISP = disp-high;disp-low
        uint16_t disp = CPU::cpu->get_word_disp();//get total disp
        uint16_t data = CPU::cpu->read_from_ram((uint16_t*)CPU::cpu->get_effective_address(fields.rm, disp));//get data to push
        CPU::cpu->push_to_stack(data);
         
    }
    else if(fields.mod == 0b11){
        //rm is threated as a reg, push a register in rm
        CPU::cpu->push_to_stack(CPU::cpu->registers[fields.rm].data); // push register to stack 
    }


    #undef MOD_L
    #undef MOD_H
    #undef RM_L
    #undef RM_H
}

void PUSH::execute_register(uint8_t opcode){
    #define REG_L 0
    #define REG_H 2

    
    uint8_t reg = CPU::cpu->get_bits(REG_L, REG_H, opcode); //get reg field
    CPU::cpu->push_to_stack(CPU::cpu->registers[reg].data); //push register 
    
    #undef REG_L
    #undef REG_H
}

void PUSH::execute_segment_register(uint8_t opcode){
    #define REG_L 3
    #define REG_H 4

    uint8_t reg = CPU::cpu->get_bits(REG_L, REG_H, opcode); // get reg field 
    CPU::cpu->push_to_stack(CPU::cpu->segment_registers[reg].data); //push segment

    #undef REG_L
    #undef REG_H
}
PUSH::~PUSH()
{
}
