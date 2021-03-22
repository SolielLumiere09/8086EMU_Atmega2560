#include <CPU.h>
#include <MOV/MOV_0_OPCODE.h>
#include <MOV/MOV_1_OPCODE.h>
#include <MOV/MOV_2_OPCODE.h>

static const uint8_t PROGRAM_MEM[SIZE_RAM] PROGMEM = {0xb8,0x45,0x23,0xbb,0x32,0x12,0xb5,0x12,0xb1,0x67};
static const char* REG_MAPS_16[]  = {"AX", "CX", "DX", "BX", "SP", "BP", "SI", "DI"};


CPU::CPU(){
    
    clear_registers();
    mov_0 = new MOV_0_OPCODE(this);    
    mov_1 = new MOV_1_OPCODE(this);
    mov_2 = new MOV_2_OPCODE(this);

}
void CPU::clear_registers(){

    PC = 0;
    segment_registers[DS_REG] = 0;
    segment_registers[CS_REG] = 0;
    segment_registers[SP_REG] = 0;
    segment_registers[ES_REG] = 0;
    registers[AX_REG].data = 0x1233;
    registers[BX_REG].data = 0X3489;
    registers[CX_REG].data = 0X7751;
    registers[DX_REG].data = 0X1E12;
    registers[DI_REG].data = 0x0032;
    registers[SI_REG].data = 0x0002;
    registers[SP_REG].data = SIZE_RAM-1;
    

}
void CPU::update(){
    uint8_t opcode = fetch();
    
    switch (decode(opcode))
    {
        case MOV_0_ID : mov_0->execute(opcode);
                        break;
        
        case MOV_1_ID : mov_1->execute(opcode);
                        break;

        case MOV_2_ID:  mov_2->execute(opcode);
                        break;
    }

}

void CPU::write_to_ram(uint16_t* address, uint16_t data){
    
    address = (uint16_t*)ADD_TO_ADDR(address);
    *address = data;

}
void CPU::write_to_ram(uint8_t* address, uint8_t data){
    address = (uint8_t*)ADD_TO_ADDR(address);
    *address = data;
}
uint8_t CPU::read_from_ram(uint8_t* address){
    address = (uint8_t*)ADD_TO_ADDR(address);
    return *address;
}
uint16_t CPU::read_from_ram(uint16_t* address){

    address = (uint16_t*)ADD_TO_ADDR(address);

    return *address;
}
uint16_t CPU::get_effective_address(uint8_t rm, uint16_t disp){

    uint16_t EA = segment_registers[DS_REG]*0x10;

    switch(rm){
        case 0b000: EA += registers[BX_REG].data + registers[SI_REG].data +  disp;   
                    break;

        case 0b001: EA += registers[BX_REG].data + registers[DI_REG].data + disp;
                    break;

        case 0b010: EA += registers[BP_REG].data + registers[SI_REG].data + disp;
                    break;

        case 0b011: EA += registers[BP_REG].data + registers[DI_REG].data + disp;
                    break;

        case 0b100: EA += registers[SI_REG].data + disp;
                    break;

        case 0b101: EA += registers[DI_REG].data + disp;
                    break;

        case 0b110: EA += registers[BP_REG].data + disp;
                    break;

        case 0b111: EA += registers[BX_REG].data + disp;
                    break;
    }

    
    
    return EA;
}
uint8_t CPU::decode(uint8_t opcode){
    
    uint8_t id = 0xff;

    if(MOV_0(opcode))
        id = MOV_0_ID;
    else if(MOV_1(opcode, pgm_read_byte(PROGRAM_MEM + PC)))
        id = MOV_1_ID;
    else if(MOV_2(opcode))
        id = MOV_2_ID;

    
    return id;
}
uint8_t CPU::fetch(){
    return pgm_read_byte(PROGRAM_MEM + PC++);
}
void CPU::print_reg_status(){
    Serial.println(REG_MAPS_16[AX_REG] + String(" = ") + String(registers[AX_REG].data, 16));
    Serial.println(REG_MAPS_16[BX_REG] + String(" = ") + String(registers[BX_REG].data, 16));
    Serial.println(REG_MAPS_16[CX_REG] + String(" = ") + String(registers[CX_REG].data, 16));
    Serial.println(REG_MAPS_16[DX_REG] + String(" = ") + String(registers[DX_REG].data, 16));
    Serial.println(REG_MAPS_16[SI_REG] + String(" = ") + String(registers[SI_REG].data, 16));
    Serial.println(REG_MAPS_16[DI_REG] + String(" = ") + String(registers[DI_REG].data, 16));
    Serial.println(REG_MAPS_16[BP_REG] + String(" = ") + String(registers[BP_REG].data, 16));
    
    
    
    Serial.println();
    
}
uint8_t CPU::get_bits(int8_t low, int8_t high, uint8_t data){
    int8_t i;
    uint8_t bits = 0;

    for(i = high; i >= low; i--){
        bits <<= 1;
        bits |= get_bit(data, i);
    }
        
    return bits;
}
bool CPU::get_bit(uint8_t data, uint8_t num_bit){
    
    return (data >> num_bit) & 1; 
}
bool CPU::LH_reg_selector(uint8_t data){

    return (data >> 2) & 1;
}
CPU::~CPU(){

}