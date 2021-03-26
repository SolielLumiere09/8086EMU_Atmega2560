#include <PUSH/PUSH_0_OPCODE.h>

PUSH_0_OPCODE::PUSH_0_OPCODE()
{
    
}
void PUSH_0_OPCODE::execute(){
    #define MOD_L 6
    #define MOD_H 7
    #define RM_L 0
    #define RM_H 2
    
    PUSH_0_FIELDS fields;
    uint8_t field_opcode = CPU::cpu->fetch();

    fields.mod = CPU::cpu->get_bits(MOD_L, MOD_H, field_opcode);
    fields.rm = CPU::cpu->get_bits(RM_L, RM_H, field_opcode);
    

    if(fields.mod == 0b00 && fields.rm == 0b110){ //disp-HIGH:disp-LOW
        uint16_t disp = CPU::cpu->get_word_disp(); //get disp 
        write_to_stack(fields.rm, disp); //write to stack
    }
    else if(fields.mod == 0b00){
        //if mod = 00 then DISP = 0*,disp-low and disp-high are absent
        write_to_stack(fields.rm, 0x00);
    }
    else if(fields.mod == 0b01){
         //if mode01 then DISP = disp-low sign-extended to 16 bits,disp-high is absent
        int16_t disp = CPU::cpu->get_word_signed_disp(); //get disp
        write_to_stack(fields.rm, disp);
    }
    else if(fields.mod == 0b10){
         //f mod = 10 then DISP = disp-high;disp-low
        uint16_t disp = CPU::cpu->get_word_disp();
        write_to_stack(fields.rm, disp); 
    }
    else if(fields.mod == 0b11){
        //rm is threated as a reg, push a register in rm
        
        CPU::cpu->write_to_ram((uint8_t*)CPU::cpu->registers[SP_REG].data - 1, CPU::cpu->registers[fields.rm].data_HL[HIGH]); //write high part
        CPU::cpu->write_to_ram((uint8_t*)CPU::cpu->registers[SP_REG].data - 2, CPU::cpu->registers[fields.rm].data_HL[LOW]); //write low part
        
        CPU::cpu->registers[SP_REG].data-=2;//decrement stack pointer
    }


    #undef MOD_L
    #undef MOD_H
    #undef RM_L
    #undef RM_H
}
void PUSH_0_OPCODE::write_to_stack(uint8_t rm, uint16_t disp){
    
    uint16_t data = CPU::cpu->read_from_ram((uint16_t*)CPU::cpu->get_effective_address(rm, disp)); //read from memory
    uint8_t dataL = (uint8_t)data;
    uint8_t dataH = (uint8_t)data >> 8;

    CPU::cpu->write_to_ram((uint8_t*)CPU::cpu->registers[SP_REG].data - 1, dataH);//write the high byte to stack 
    CPU::cpu->write_to_ram((uint8_t*)CPU::cpu->registers[SP_REG].data - 2, dataL);//write the low byte to stack 
    

    CPU::cpu->registers[SP_REG].data-=2;//decrement stack pointer
}

PUSH_0_OPCODE::~PUSH_0_OPCODE()
{
}
