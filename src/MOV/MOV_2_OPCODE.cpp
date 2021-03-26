#include <MOV/MOV_2_OPCODE.h>

MOV_2_OPCODE::MOV_2_OPCODE(){

}
void MOV_2_OPCODE::execute(uint8_t opcode){ //Immediate to Register
    #define w_field 3
    #define reg_fieldL 0
    #define reg_fieldH 2
    
    
    MOV_2_FIELDS fields;

    fields.w = CPU::cpu->get_bit(opcode, w_field);
    fields.reg = CPU::cpu->get_bits(reg_fieldL, reg_fieldH, opcode);

    if(fields.w){
        //word instruction
        uint16_t data = CPU::cpu->get_word();//get data 
        CPU::cpu->registers[fields.reg].data = data; //copy data 

    }
    else{
        //byte instruction
        uint8_t data = CPU::cpu->get_byte();
        CPU::cpu->registers[fields.reg % 4].data_HL[CPU::cpu->LH_reg_selector(fields.reg)] = data;
    }


    #undef w_field
    #undef reg_field

}
MOV_2_OPCODE::~MOV_2_OPCODE(){

}