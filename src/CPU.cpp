#include <CPU.h>

static const uint8_t PROGRAM_MEM[SIZE_RAM] PROGMEM = {0xc7, 0xc1, 0x32, 0x52};
static const char* REG_MAPS_16[]  = {"AX", "CX", "DX", "BX", "SP", "BP", "SI", "DI"};


CPU::CPU(){
    
    clear_registers();
    

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
    registers[SP_REG].data = SIZE_RAM-1;
    

}
void CPU::update(){
    uint8_t opcode = fetch();
    
    switch (decode(opcode))
    {
        case MOV_0_ID : execute_MOV_0(opcode);
                        break;
        
        case MOV_1_ID : execute_MOV_1(opcode);
                        break;  
    }

}
void CPU::execute_MOV_0(uint8_t opcode){

    DATA_TRANSFER_FIELD fields;
    uint8_t fields_opcode = fetch();

    fields.w = opcode >> w_field;
    fields.d = opcode >> d_field;
    fields.mod = fields_opcode >> mod_field;
    fields.rm = fields_opcode >> rm_field;
    fields.reg = fields_opcode >> reg_field;

    if(fields.mod == 0b11){
        //rm treated like a reg 
        if(fields.w){
            //word instruction
            if(fields.d){
                //rm to reg
                registers[fields.reg].data = registers[fields.rm].data;
            }
            else{
                //reg to rm
                registers[fields.rm].data = registers[fields.reg].data;
            }
        }
        else{
            //byte instruction
            if(fields.d){
                //rm to reg
                registers[fields.reg % 4].data_HL[LH_reg_v] = registers[fields.rm % 4].data_HL[LH_rm_v];
            
            }
            else{
                //reg to rm
                registers[fields.rm % 4].data_HL[LH_rm_v] = registers[fields.reg % 4].data_HL[LH_reg_v];

            }
        }

    }
    else if(fields.mod == 0b00 && fields.rm == 0b110){
         //if mod 00 and r/m = 110 then EA = disp-high;disp-low.

        uint16_t dispL = fetch(); //get Low disp
        uint16_t dispH = fetch(); //get High dip
        uint16_t disp = (segment_registers[DS_REG]*0x10) + ((dispH << 8) | dispL);
        write_or_read_ram(disp, &fields);

    }
    else if(fields.mod == 0b00){
        //if mod=00 then DISP=0*, disp-io and disp-high are absent
        uint16_t disp = get_effective_address(fields.rm, 0);
        write_or_read_ram(disp, &fields);
    }
    else if(fields.mod == 0b01){
        //if mod=01 then DISP= disp-low sign-extended to 16 bits,disp-highis absent
        int16_t dispL = (int16_t)fetch(); //get low disp
        uint16_t disp = get_effective_address(fields.rm, (uint16_t)dispL);

        write_or_read_ram(disp, &fields);
    }
    else if (fields.mod == 0b10){
        uint16_t dispL = fetch(); //get Low disp
        uint16_t dispH = fetch(); //get High dip
        uint16_t dispT = ((dispH << 8) | dispL);//complete disp
        uint16_t disp = get_effective_address(fields.rm, dispT);// effective address
        write_or_read_ram(disp, &fields);
    }
    
}

void CPU::execute_MOV_1(uint8_t opcode){
    
    DATA_TRANSFER_FIELD fields;
    uint8_t fields_opcode = fetch();

    fields.w = opcode >> w_field;
    fields.mod = fields_opcode >> mod_field;
    fields.rm = fields_opcode >> rm_field;

    if(fields.mod == 0b11){
        //inmediate to register 
        if(fields.w){
            //word instruction
            uint16_t dataL = fetch();
            uint16_t dataH = fetch();
            uint16_t data = (dataH << 8) | dataL;

            registers[fields.rm].data = data;

        }
        else{
            //byte instruction
            uint8_t data = fetch();

            registers[fields.rm % 4].data_HL[LH_rm_v] = data;
        }

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
                write_to_ram((uint8_t*)disp, registers[fields->reg % 4].data_HL[LH_reg_r]);
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
                registers[fields->reg].data_HL[LH_reg_r] = read_from_ram((uint8_t*)disp);
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
CPU::~CPU(){

}