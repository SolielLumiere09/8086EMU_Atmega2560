#include <MOV/MOV_4_OPCODE.h>



MOV_4_OPCODE::MOV_4_OPCODE(){
 
}
void MOV_4_OPCODE::execute(uint8_t opcode){ //Accumulator to Memory
    #define w_bit 0
    
    bool w = CPU::cpu->get_bit(opcode, w_bit); // get the w bit
    uint16_t disp = CPU::cpu->get_word_disp(); //get disp

    if(w){
        //word instruction
        CPU::cpu->write_to_ram((uint16_t*)disp, CPU::cpu->registers[AX_REG].data);

    }
    else{
        //byte instruction
        CPU::cpu->write_to_ram((uint8_t*)disp, CPU::cpu->registers[AX_REG].data_HL[LOW]);
    }


    #undef w_bit

}
MOV_4_OPCODE::~MOV_4_OPCODE(){

}
