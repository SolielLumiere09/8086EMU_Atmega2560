#include <XCHG.h>

XCHG::XCHG(/* args */)
{
}
void XCHG::execute_register_or_memory_with_register(uint8_t opcode){
    #define w_bit 0
    #define MOD_H 7
    #define MOD_L 6
    #define REG_H 5
    #define REG_L 3
    #define RM_H 2
    #define RM_L 0

    xchg_register_or_memory_with_register_fields fields;
    uint8_t opcode_fields = CPU::cpu->get_byte();

    fields.w = CPU::cpu->get_bit(opcode, w_bit); //get w field 
    fields.rm = CPU::cpu->get_bits(RM_L, RM_H, opcode_fields);//get rm field
    fields.reg = CPU::cpu->get_bits(REG_L, REG_H, opcode_fields); //get reg field
    fields.mod = CPU::cpu->get_bits(MOD_L, MOD_H, opcode_fields);//get mod field 

    if(fields.mod == 0b00 && fields.rm == 0b110){
        //if mod 00 and r/m = 110 then EA = disp-high;disp-low.
        uint16_t disp = CPU::cpu->get_word_disp(DS_REG); // get total disp
        
        if(fields.w){
            //word instruction
            CPU::cpu->swap((uint16_t*)disp, &CPU::cpu->registers[fields.reg].data); //xchg with 16 bit register 

        }
        else {
            //byte instruction
            CPU::cpu->swap((uint8_t*)disp, &CPU::cpu->registers[fields.reg % 4].data_HL[CPU::cpu->LH_reg_selector(fields.reg)]); //xchg with 8 bit register
        
        }

    }
    else if(fields.mod == 0b00){
        //if mod=00 then DISP=0*, disp-io and disp-high are absent
        if(fields.w){
            //word instruction
            CPU::cpu->swap((uint16_t*)CPU::cpu->get_effective_address(fields.rm, 0), &CPU::cpu->registers[fields.reg].data);//swap with a word register
        }
        else{
            //byte instruction
            CPU::cpu->swap((uint8_t*)CPU::cpu->get_effective_address(fields.rm, 0), &CPU::cpu->registers[fields.reg % 4].data_HL[CPU::cpu->LH_reg_selector(fields.reg)]); //xchg with 8 bit register
        
        }
    }
    else if(fields.mod == 0b01){
        //if mod=01 then DISP= disp-low sign-extended to 16 bits,disp-highis absent
        int16_t disp = CPU::cpu->get_word_signed_disp(); //get disp

        if(fields.w){
            //word instruction
            CPU::cpu->swap((uint16_t*)CPU::cpu->get_effective_address(fields.rm, (uint16_t)disp), &CPU::cpu->registers[fields.reg].data); //swap with word register
        }
        else{
            //byte instruction
            CPU::cpu->swap((uint8_t*)CPU::cpu->get_effective_address(fields.rm, (uint16_t)disp), &CPU::cpu->registers[fields.reg % 4].data_HL[CPU::cpu->LH_reg_selector(fields.reg)]); //xchg with 8 bit register
        
        }

    }
    else if(fields.mod == 0b10){
         //if mod=10 then DISP= disp-high:disp-low
        uint16_t disp = CPU::cpu->get_word_disp(); // get disp
        if(fields.w){
            //word instruction
            CPU::cpu->swap((uint16_t*)CPU::cpu->get_effective_address(fields.rm, disp), &CPU::cpu->registers[fields.reg].data); //swap with word register
        }
        else{
            //byte instruction
            CPU::cpu->swap((uint8_t*)CPU::cpu->get_effective_address(fields.rm, disp), &CPU::cpu->registers[fields.reg % 4].data_HL[CPU::cpu->LH_reg_selector(fields.reg)]); //xchg with 8 bit register
        
        }
    }
    else if(fields.mod == 0b11){
        //rm treated like a reg 
        if(fields.w){
            //word instruction
            CPU::cpu->swap(&CPU::cpu->registers[fields.reg].data, &CPU::cpu->registers[fields.rm].data); //swap word registers
        }
        else {
            //byte instruction
            CPU::cpu->swap(&CPU::cpu->registers[fields.reg % 4].data_HL[CPU::cpu->LH_reg_selector(fields.reg)], &CPU::cpu->registers[fields.rm % 4].data_HL[CPU::cpu->LH_reg_selector(fields.rm)]);
        }
    }

    #undef w_bit
    #undef MOD_H
    #undef MOD_L
    #undef REG_H
    #undef REG_L
    #undef RM_H     
    #undef RM_L 
}
void XCHG::execute_register_with_accumulator(uint8_t opcode){
    #define REG_H 2
    #define REG_L 0

    uint8_t reg = CPU::cpu->get_bits(REG_L, REG_H, opcode); //get reg field
    CPU::cpu->swap(&CPU::cpu->registers[AX_REG].data, &CPU::cpu->registers[reg].data); //swap register with acumulator 

    #undef REG_H 
    #undef REG_L 

}
XCHG::~XCHG()
{
}
