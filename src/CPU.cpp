#include <CPU.h>
#include <MOV/MOV_0_OPCODE.h>
#include <MOV/MOV_1_OPCODE.h>

static const uint8_t PROGRAM_MEM[SIZE_RAM] PROGMEM = {0xc6, 0x40, 0x12, 0x33, 0x8b, 0x58, 0x12};
static const char* REG_MAPS_16[]  = {"AX", "CX", "DX", "BX", "SP", "BP", "SI", "DI"};


CPU::CPU(){
    
    clear_registers();
    mov_0 = new MOV_0_OPCODE(this);    
    mov_1 = new MOV_1_OPCODE(this);

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
    }

}

void CPU::write_or_read_ram(uint16_t disp, DATA_TRANSFER_FIELD *fields){

     if(fields->d == 0){
            //write data to reg
            if(fields->w){
                //write 16 bits reg
                write_to_ram((uint16_t*)disp, registers[fields->reg].data);
            }
            else{
                //write 8 bits reg
                
                write_to_ram((uint8_t*)disp, registers[fields->reg % 4].data_HL[LH_reg_selector(fields->reg)]);
            }
        }
        else{
            //read data to reg
            if(fields->w){
                //read a word 
                registers[fields->reg % 4].data = read_from_ram((uint16_t*)disp);

            }
            else{
                //read a byte
                registers[fields->reg % 4].data_HL[LH_reg_selector(fields->reg)] = read_from_ram((uint8_t*)disp);
            }
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
    Serial.println();
    
}
bool CPU::LH_reg_selector(uint8_t data){

    return (data >> 2) & 1;
}
CPU::~CPU(){

}