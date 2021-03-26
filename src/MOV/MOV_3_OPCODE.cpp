#include <MOV/MOV_3_OPCODE.h>



MOV_3_OPCODE::MOV_3_OPCODE(){
    
}
void MOV_3_OPCODE::execute(uint8_t opcode){ //Memory to Accumulator
    #define w_field 0

    bool w = CPU::cpu->get_bit(opcode, w_field);
    uint16_t disp = CPU::cpu->get_word_disp(); //get disp

    if(w){
        //word instruction
        CPU::cpu->registers[AX_REG].data = CPU::cpu->read_from_ram((uint16_t*)disp); //read a word from ram 

    }
    else{
        //byte instruction
        CPU::cpu->registers[AX_REG].data_HL[LOW] = CPU::cpu->read_from_ram((uint8_t*)disp); //read a byte
    }


    #undef w_field
}
MOV_3_OPCODE::~MOV_3_OPCODE(){

}