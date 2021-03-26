#include <POP/POP_0_OPCODE.h>

POP_0_OPCODE::POP_0_OPCODE(/* args */)
{

}
void POP_0_OPCODE::execute(){
    #define MOD_L 6
    #define MOD_H 7
    #define RM_L 0
    #define RM_H 2

    uint8_t opcode_fields = CPU::cpu->fetch(); //get fields 
    POP_0_FIELDS fields;

    fields.mod = CPU::cpu->get_bits(MOD_L, MOD_H, opcode_fields); // get mod field
    fields.rm = CPU::cpu->get_bits(RM_L, RM_H, opcode_fields);// get rm field

    if(fields.mod == 0b00 && fields.rm == 0b110){
        //except if mod = 00 and r/m = 110 then EA = disp-high;disp-low
        uint16_t disp = (uint16_t)CPU::cpu->fetch(); // get disp-low
        disp |= (uint16_t)CPU::cpu->fetch() << 8; //get disp-high

        uint16_t data = CPU::cpu->read_from_ram((uint16_t*)CPU::cpu->registers[SP_REG].data);
        
        CPU::cpu->write_to_ram((uint16_t*)disp, data);//pop to memory
        CPU::cpu->registers[SP_REG].data += 2; //increment stack pointer


    }

    #undef MOD_L
    #undef MOD_H
    #undef RM_L
    #undef RM_H
}
POP_0_OPCODE::~POP_0_OPCODE()
{
}