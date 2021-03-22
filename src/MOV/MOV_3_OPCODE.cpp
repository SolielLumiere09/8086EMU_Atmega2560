#include <MOV/MOV_3_OPCODE.h>



MOV_3_OPCODE::MOV_3_OPCODE(CPU *cpu){
    this->cpu = cpu;
}
void MOV_3_OPCODE::execute(uint8_t opcode){ //Memory to Accumulator
    #define w_field 0

    bool w = cpu->get_bit(opcode, w_field);
    uint16_t disp;

    disp = (uint16_t)cpu->fetch(); //get dispL
    disp |= ((uint16_t)cpu->fetch()) << 8; //get dispH

    if(w){
        //word instruction
        cpu->registers[AX_REG].data = cpu->read_from_ram((uint16_t*)disp); //read a word from ram 

    }
    else{
        //byte instruction
        cpu->registers[AX_REG].data_HL[LOW] = cpu->read_from_ram((uint8_t*)disp); //read a byte
    }


    #undef w_field
}
MOV_3_OPCODE::~MOV_3_OPCODE(){

}