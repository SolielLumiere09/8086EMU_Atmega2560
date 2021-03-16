#include <CPU.h>

static const uint8_t RAM[SIZE_RAM] PROGMEM = {0x88, 0xDC, 0x66, 0x89, 0xD8, 0x88, 0xF4, 0x88, 0xC8, 0x66, 0x89, 0xD1};
static const char* REG_MAPS_16[]  = {"AX", "CX", "DX", "BX", "SP", "BP", "SI", "DI"};
static const char* REG_MAPS_8[] = {"AL", "CL", "DL", "BL", "AH", "CH", "DH", "BH"};



CPU::CPU(){
    
    registers[SP_REG].data = SIZE_RAM-1;
    PC = 0;

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




    #undef LH_reg
    #undef LH_rm

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