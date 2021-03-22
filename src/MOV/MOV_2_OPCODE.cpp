#include <MOV/MOV_2_OPCODE.h>

MOV_2_OPCODE::MOV_2_OPCODE(CPU *cpu){
    this->cpu = cpu;
}
void MOV_2_OPCODE::execute(uint8_t opcode){ //Immediate to Register
    #define w_field 3
    #define reg_fieldL 0
    #define reg_fieldH 2
    
    
    MOV_2_FIELDS fields;

    fields.w = cpu->get_bit(opcode, w_field);
    fields.reg = cpu->get_bits(reg_fieldL, reg_fieldH, opcode);

    if(fields.w){
        //word instruction
        uint16_t data = (uint16_t)cpu->fetch();
        data |= (uint16_t)cpu->fetch() << 8;

        cpu->registers[fields.reg].data = data;

    }
    else{
        //byte instruction
        uint8_t data = cpu->fetch();
        cpu->registers[fields.reg % 4].data_HL[cpu->LH_reg_selector(fields.reg)] = data;
    }


    #undef w_field
    #undef reg_field

}
MOV_2_OPCODE::~MOV_2_OPCODE(){

}