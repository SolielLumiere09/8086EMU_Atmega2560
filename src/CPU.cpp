#include <CPU.h>
#include <MOV/MOV_0_OPCODE.h>
#include <MOV/MOV_1_OPCODE.h>
#include <MOV/MOV_2_OPCODE.h>
#include <MOV/MOV_3_OPCODE.h>
#include <MOV/MOV_4_OPCODE.h>
#include <PUSH/PUSH_0_OPCODE.h>
#include <PUSH/PUSH_1_OPCODE.h>
#include <PUSH/PUSH_2_OPCODE.h>

static const uint8_t PROGRAM_MEM[SIZE_RAM] PROGMEM = {0xb8,0x56,0x34,0xa3,0x34,0x22,0x8b,0x16,0x34,0x22,0x89,0xd1,0xb8,0x55,0x00,0x88,0x00,0x8a,0x10,0x88,0x6f,0x12,0x88,0xec};
static const char* REG_MAPS_16[]  = {"AX", "CX", "DX", "BX", "SP", "BP", "SI", "DI"};
CPU* CPU::cpu;

CPU::CPU(){
    CPU::cpu = this;
    clear_registers();
    mov_0 = new MOV_0_OPCODE();    
    mov_1 = new MOV_1_OPCODE();
    mov_2 = new MOV_2_OPCODE();
    mov_3 = new MOV_3_OPCODE();
    mov_4 = new MOV_4_OPCODE();
    push_0 = new PUSH_0_OPCODE();
    push_1 = new PUSH_1_OPCODE();

}
void CPU::clear_registers(){

    PC = 0;
    segment_registers[DS_REG].data = 0;
    segment_registers[CS_REG].data = 0;
    segment_registers[SP_REG].data = 0;
    segment_registers[ES_REG].data = 0;
    registers[AX_REG].data = 0x1233;
    registers[BX_REG].data = 0X3489;
    registers[CX_REG].data = 0X7751;
    registers[DX_REG].data = 0X1E12;
    registers[DI_REG].data = 0x2332;
    registers[SI_REG].data = 0x1321;
    registers[BP_REG].data = 0x1212;
    registers[SP_REG].data = 0xFFFF;
    

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
        
        case MOV_3_ID:  mov_3->execute(opcode);
                        break;

        case MOV_4_ID:  mov_4->execute(opcode);
                        break;

        case PUSH_0_ID: push_0->execute();
                        break;

        case PUSH_1_ID: push_1->execute(opcode);
                        break;

        case PUSH_2_ID: push_2->execute(opcode);
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
uint16_t CPU::get_word_disp(){
    uint16_t EA = (uint16_t)fetch(); //get low disp
    EA |= (uint16_t)fetch() << 8;// get high disp

    EA += segment_registers[DS_REG].data * 0x10; //get effective address 

    return EA;
}
int16_t CPU::get_word_signed_disp(){
    int16_t EA = (int16_t)fetch(); //get disp and extend the sign 

    return EA;
}
uint8_t CPU::get_byte(){

    return fetch();
}
uint16_t CPU::get_word(){
    uint16_t data = (uint16_t)fetch();
    data |= (uint16_t)fetch() << 8;


    return data;
}
uint16_t CPU::get_effective_address(uint8_t rm, uint16_t disp){

    uint16_t EA = segment_registers[DS_REG].data * 0x10;

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

    if(MOV_1(opcode, pgm_read_byte(PROGRAM_MEM + PC)))
        id = MOV_1_ID;
    if(PUSH_0(opcode, pgm_read_byte(PROGRAM_MEM + PC)))
        id = PUSH_0_ID;
    else if(MOV_0(opcode))
        id = MOV_0_ID;
    else if(MOV_2(opcode))
        id = MOV_2_ID;
    else if(MOV_3(opcode))
        id = MOV_3_ID;
    else if(MOV_4(opcode))
        id = MOV_4_ID;
    else if(PUSH_1(opcode))
        id = PUSH_1_ID;
    else if(PUSH_2(opcode))
        id = PUSH_2_ID;

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