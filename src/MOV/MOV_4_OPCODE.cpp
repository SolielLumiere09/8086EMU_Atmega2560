#include <MOV/MOV_4_OPCODE.h>



MOV_4_OPCODE::MOV_4_OPCODE(CPU *cpu){
    this->cpu = cpu;
}
void MOV_4_OPCODE::execute(uint8_t opcode){ //Accumulator to Memory
    #define w_bit 0
    
    bool w = cpu->get_bit(opcode, w_bit); // get the w bit
    
    uint16_t disp = (uint16_t)cpu->fetch(); // get de low address
    disp |= (uint16_t)cpu->fetch() << 8; //obtain de complete address

    if(w){
        //word instruction
        cpu->write_to_ram((uint16_t*)disp, cpu->registers[AX_REG].data);

    }
    else{
        //byte instruction
        cpu->write_to_ram((uint8_t*)disp, cpu->registers[AX_REG].data_HL[LOW]);
    }


    #undef w_bit

}
MOV_4_OPCODE::~MOV_4_OPCODE(){

}
