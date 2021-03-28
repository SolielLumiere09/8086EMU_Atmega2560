#include <CPU.h>
#include <MOV.h>
#include <PUSH.h>
#include <POP.h>
#include <XCHG.h>

static const uint8_t PROGRAM_MEM[SIZE_RAM] PROGMEM = {0x93,0x86,0xea,0x87,0x06,0x44,0x44,0x87,0x1e,0x44,0x44};
static const char* REG_MAPS_16[]  = {"AX", "CX", "DX", "BX", "SP", "BP", "SI", "DI"};
CPU* CPU::cpu;

CPU::CPU(){
    CPU::cpu = this;
    clear_registers();
    this->executed = false;

    this->mov = new MOV();
    this->push = new PUSH();
    this->pop = new POP();
    this->xchg = new XCHG();

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
    //fetxh instruction
    uint8_t opcode = fetch();

    mov_execute(opcode);
    push_execute(opcode);
    pop_execute(opcode);
    xchg_execute(opcode);
    executed = false;
}
void CPU::mov_execute(uint8_t opcode){
    if(!executed){
        if(is_mov_inmediate_to_register_or_memory(opcode, peek_next_byte())){
            mov->execute_inmediate_to_register_or_memory(opcode);
            executed = true;
        }
        else if(is_mov_register_or_memory_to_or_from_register(opcode)){
            mov->execute_mov_register_or_memory_to_or_from_register(opcode);
            executed = true;
        }
        else if(is_mov_inmediate_to_register(opcode)){
            mov->execute_inmediate_to_register(opcode);
            executed = true;
        }
        else if(is_mov_memory_to_accumulator(opcode)){
            mov->execute_memory_to_accumulator(opcode);
            executed = true;
        }
        else if(is_mov_accumulator_to_memory(opcode)){
            mov->execute_accumulator_to_memory(opcode);
            executed = true;
        }
    }
}
void CPU::push_execute(uint8_t opcode){
    if(!executed){
        if(is_push_register_or_memory(opcode, peek_next_byte())){
            executed = true;
            push->execute_register_or_memory();
        }
        else if(is_push_register(opcode)){
            executed = true;
            push->execute_register(opcode);
        }
        else if(is_push_segment(opcode)){
            executed = true;
            push->execute_segment_register(opcode);
        }
        
    }

}

void CPU::pop_execute(uint8_t opcode){
    if(!executed){
        if(is_pop_register_or_memory(opcode, peek_next_byte())){
            executed = true;
            pop->execute_pop_register_or_memory(opcode);
        }
        else if(is_pop_register(opcode)){
            executed = true;
            pop->execute_pop_register(opcode);
        }
        else if(is_pop_segment(opcode)){
            executed = true;
            pop->execute_pop_segment_register(opcode);
        }
    }
}
void CPU::xchg_execute(uint8_t opcode){
    if(!executed){
        if(is_xchg_register_or_memory_with_register(opcode)){
            executed = true;
            xchg->execute_register_or_memory_with_register(opcode);
        }
        else if(is_xchg_register_with_accumulator(opcode)){
            executed = true;
            xchg->execute_register_with_accumulator(opcode);
        }
    }

}
void CPU::swap(uint16_t* x, uint16_t* y){
    uint16_t aux;

    aux = *x; //store the value of x temporaly
    *x = *y; //move the content of y into x 
    *y = aux;//store the content of x into y
}
void CPU::swap(uint8_t* x, uint8_t* y){
    uint8_t aux;

    aux = *x; //store the value of x temporaly
    *x = *y; //move the content of y into x 
    *y = aux;//store the content of x into y
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
uint16_t CPU::get_word_disp(uint8_t segment){
    uint16_t EA = (uint16_t)fetch(); //get low disp
    EA |= (uint16_t)fetch() << 8;// get high disp
    EA += segment_registers[segment].data; //add the segment 
    return EA;
}
uint16_t CPU::get_word_disp(){
    uint16_t EA = (uint16_t)fetch(); //get low disp
    EA |= (uint16_t)fetch() << 8;// get high disp

    return EA;
}
int16_t CPU::get_word_signed_disp(){
    int16_t EA = (int16_t)fetch(); //get disp and extend the sign 
    
    return EA;
}
void CPU::push_to_stack(uint16_t data){
    uint16_t disp = ((segment_registers[SS_REG].data << 4) + registers[SP_REG].data); //get total disp of stack
    uint8_t dataL = (uint8_t)(data & 0x00FF); //low part
    uint8_t dataH = (uint8_t)(data >> 8);// high part

    write_to_ram((uint8_t*)disp - 1, dataH); //write high part of data
    write_to_ram((uint8_t*)disp - 2, dataL); //write low part of data

    registers[SP_REG].data -= 2; //substract stack pointer


} 
uint16_t CPU::pop_from_stack(){
    uint16_t disp = ((segment_registers[SS_REG].data << 4) + registers[SP_REG].data); //get total disp 
    uint16_t data = read_from_ram((uint16_t*)disp); //get data

    registers[SP_REG].data += 2;

    return data;
}
uint8_t CPU::get_byte(){

    return fetch();
}
uint16_t CPU::get_word(){
    uint16_t data = (uint16_t)fetch();
    data |= (uint16_t)fetch() << 8;


    return data;
}
uint8_t CPU::peek_next_byte(){
    return pgm_read_byte(PROGRAM_MEM + PC);
}
uint16_t CPU::get_effective_address(uint8_t rm, uint16_t disp){

    uint16_t EA = 0;

    switch(rm){
        case 0b000: EA = (segment_registers[DS_REG].data << 4) + registers[BX_REG].data + registers[SI_REG].data +  disp;   
                    break;

        case 0b001: EA = (segment_registers[DS_REG].data << 4) + registers[BX_REG].data + registers[DI_REG].data + disp;
                    break;

        case 0b010: EA = (segment_registers[SS_REG].data << 4) + registers[BP_REG].data + registers[SI_REG].data + disp;
                    break;

        case 0b011: EA = (segment_registers[SS_REG].data << 4) + registers[BP_REG].data + registers[DI_REG].data + disp;
                    break;

        case 0b100: EA = (segment_registers[DS_REG].data << 4) + registers[SI_REG].data + disp;
                    break;

        case 0b101: EA = (segment_registers[DS_REG].data << 4) + registers[DI_REG].data + disp;
                    break;

        case 0b110: EA = (segment_registers[SS_REG].data << 4) + registers[BP_REG].data + disp;
                    break;

        case 0b111: EA = (segment_registers[DS_REG].data << 4) + registers[BX_REG].data + disp;
                    break;
    }

    
    
    return EA;
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