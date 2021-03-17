#include <CPU.h>

static const uint8_t RAM[SIZE_RAM] PROGMEM = {0x66, 0x8B, 0x1D, 0x34, 0x12, 0x00, 0x00};
static const char* REG_MAPS_16[]  = {"AX", "CX", "DX", "BX", "SP", "BP", "SI", "DI"};
static const char* REG_MAPS_8[] = {"AL", "CL", "DL", "BL", "AH", "CH", "DH", "BH"};



CPU::CPU(){
    registers[SP_REG].data = SIZE_RAM-1;
    PC = 0;

}
void CPU::clear_registers(){
    segment_registers[DS_REG] = 0;
}
void CPU::update(){
    uint8_t opcode = fetch();
    
    switch (decode(opcode))
    {
        case MOV_0_ID : MOV_0_FIELDS fields;
                        uint8_t fields_opcode = fetch();
                        fields.w = opcode >> w_field;
                        fields.d = opcode >> d_field;
                        fields.mod = fields_opcode >> mod_field;
                        fields.rm = fields_opcode >> rm_field;
                        fields.reg = fields_opcode >> reg_field;
                        execute_MOV_0(&fields);
                        break;
    }

}
void CPU::execute_MOV_0(MOV_0_FIELDS *fields){
    #define LH_reg (fields->reg >> 2)
    #define LH_rm (fields->rm >> 2)

    if(fields->mod == 0b11){
        //rm treated like a reg 
        if(fields->w){
            //word instruction
            if(fields->d){
                //rm to reg
                registers[fields->reg].data = registers[fields->rm].data;
                #ifdef MOV_0_G
                Serial.print(F("MOV "));
                Serial.print(REG_MAPS_16[fields->reg]);
                Serial.print(F(", "));
                Serial.println(REG_MAPS_16[fields->rm]);
                #endif
            }
            else{
                //reg to rm
                registers[fields->rm].data = registers[fields->reg].data;
                #ifdef MOV_0_G
               
                Serial.print(F("MOV "));
                Serial.print(REG_MAPS_16[fields->rm]);
                Serial.print(F(", "));
                Serial.println(REG_MAPS_16[fields->reg]);
                #endif
            }
        }
        else{
            //byte instruction
            if(fields->d){
                //rm to reg
                registers[fields->reg % 4].data_HL[LH_reg] = registers[fields->rm % 4].data_HL[LH_rm];
                #ifdef MOV_0_G
                Serial.print(F("MOV "));
                Serial.print(REG_MAPS_8[fields->reg]);
                Serial.print(F(", "));
                Serial.println(REG_MAPS_8[fields->rm]);
                #endif
            
            }
            else{
                //reg to rm
                registers[fields->rm % 4].data_HL[LH_rm] = registers[fields->reg % 4].data_HL[LH_reg];
                #ifdef MOV_0_G
                Serial.print(F("MOV "));
                Serial.print(REG_MAPS_8[fields->rm]);
                Serial.print(F(", "));
                Serial.println(REG_MAPS_8[fields->reg]);
                #endif

            }
        }

    }
    else if(fields->mod == 0b00 && fields->rm == 0b110){
         //if mod 00 and r/m = 110 then EA = disp-high;disp-low.

        uint16_t dispL = fetch(); //get Low disp
        uint16_t dispH = fetch(); //get High dip
        uint16_t disp = (segment_registers[DS_REG]*0x10) + ((dispH << 8) | dispL);



        if(fields->d == 0){
            //write data to reg
            if(fields->w){
                //write 16 bits reg
                write_to_ram((uint16_t*)disp, registers[fields->reg].data);
            }
            else{
                //write 8 bits reg
                write_to_ram((uint8_t*)disp, registers[fields->reg % 4].data_HL[LH_reg]);
            }
        }
        else{
            //read data to reg
            if(fields->w){
                //read a word 
                registers[fields->reg].data = read_from_ram((uint16_t*)disp);

            }
            else{
                //read a byte
                registers[fields->reg].data_HL[LH_reg] = read_from_ram((uint8_t*)disp);
            }
        }

    }

    


    #undef LH_reg
    #undef LH_rm

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

    uint16_t EA = 0;

    switch(rm){
        case 0b000: EA = registers[BX_REG].data + registers[SI_REG].data +  disp;   
                    break;

        case 0b001: EA = registers[BX_REG].data + registers[DI_REG].data + disp;
                    break;

        case 0b010: EA = registers[BP_REG].data + registers[SI_REG].data + disp;
                    break;

        case 0b011: EA = registers[BP_REG].data + registers[DI_REG].data + disp;
                    break;

        case 0b100: EA = registers[SI_REG].data + disp;
                    break;

        case 0b101: EA = registers[DI_REG].data + disp;
                    break;

        case 0b110: EA = registers[BP_REG].data + disp;
                    break;

        case 0b111: EA = registers[BX_REG].data + disp;
                    break;
    }

    return EA;
}
uint8_t CPU::decode(uint8_t opcode){
    if(MOV_0(opcode))
        return MOV_0_ID;


    return 0xFF;
}
uint8_t CPU::fetch(){
    return pgm_read_byte(RAM + PC++);
}
CPU::~CPU(){

}