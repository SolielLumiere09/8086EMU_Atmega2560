#pragma once
#include <Arduino.h>


#define AX_REG 0
#define CX_REG 1
#define DX_REG 2
#define BX_REG 3
#define SP_REG 4
#define BP_REG 5
#define SI_REG 6
#define DI_REG 7
#define AL_REG 0
#define CL_REG 1
#define DL_REG 2
#define BL_REG 3
#define AH_REG 4
#define CH_REG 5
#define DH_REG 6
#define BH_REG 7
#define ES_REG 0
#define CS_REG 1
#define SS_REG 2
#define DS_REG 3
#define TOTAL_REGS 8 
#define TOTAL_SEGMENT_REGS 4
#define SIZE_RAM 100


#define OFFSET 0
#define ADD_TO_ADDR(address) (((uint16_t)address)+OFFSET)

//MOV
#define is_mov_register_or_memory_to_or_from_register(opcode) ((opcode & 0b11111100) == 0b10001000) //Register/Memory to/from Register
#define is_mov_inmediate_to_register_or_memory(opcode, reg) ((opcode & 0b11111110) == 0b11000110) && ((((~reg) >> 3) & 7))//Inmediate to Register/Memory
#define is_mov_inmediate_to_register(opcode) ((opcode & 0b11110000) == 0b10110000) //Immediate to Register
#define is_mov_memory_to_accumulator(opcode) ((opcode & 0b11111110) == 0b10100000) ////Memory to Accumulator
#define is_mov_accumulator_to_memory(opcode) ((opcode & 0b11111110) == 0b10100010) //Accumulator to Memory

//PUSH
#define is_push_register_or_memory(opcode, reg) ((opcode == 0b11111111) && ((reg & 0b00111000)) == 0b00110000) //Register/Memory 
#define is_push_register(opcode) ((opcode & 0b11111000) == 0b01010000)//Push register
#define is_push_segment(opcode) ((opcode & 0b11100111) == 0b00000111)//push segment register


//POP
#define is_pop_register_or_memory(opcode, field) ((opcode == 0b10001111) && ((field & 0b00111000) == 0b0000000)) //pop Register/Memory
#define is_pop_register(opcode) ((opcode & 0b11111000) == 0b01011000)//pop register
#define is_pop_segment(opcode) ((opcode & 0b11100111) == 0b00000111)//pop segment 


typedef union 
{
    uint16_t data;
    uint8_t data_HL[2];
    
}REG;


typedef union {
    uint16_t status_reg_data;
    unsigned CF : 1;//carry flag
    unsigned PF : 1;//parity flag
    unsigned AF : 1;//auxiliary flag
    unsigned ZF : 1;//zero flag
    unsigned SF : 1;//sign flag
    unsigned TF : 1;//
    unsigned IF : 1;//intrerrupt flag
    unsigned DF : 1;
    unsigned OF : 1;//overflow


}STATUS_REG; //flags of the status register

class MOV;
class PUSH;
class POP;

class CPU{
    private:
        bool executed;
        MOV *mov;
        PUSH *push;
        POP *pop;

    public:
        static CPU *cpu;
        uint16_t PC = 0; // program counter
        STATUS_REG sreg = {0}; //status register
        REG registers[TOTAL_REGS];//all reguisters
        REG segment_registers[TOTAL_REGS];//all segment registers
    
        CPU();
        uint8_t fetch();//fetch instruction from memory
        void update();//update the cpu (decode and execute)
        void mov_execute(uint8_t opcode); //for mov instructions
        void push_execute(uint8_t opcode); //for push instruction 
        void pop_execute(uint8_t opcode);//for pops instructions

        uint16_t get_effective_address(uint8_t rm, uint16_t disp); //return the effective address acording to datasheet
        uint16_t get_word_disp(); //return the effective address acording to mod = 00 and rm = 110
        uint16_t get_word_disp(uint8_t segment); //return with segment
        int16_t get_word_signed_disp(); //return the sign extended byte 
        uint8_t get_byte(); //return a byte from fetch
        uint16_t get_word();//return a word from fetch
        void write_to_ram(uint16_t* address, uint16_t data);//write a word into the ram
        void write_to_ram(uint8_t* address, uint8_t data);//wrrite a byte into the ram
        uint16_t read_from_ram(uint16_t* address);//read a word from ram
        uint8_t read_from_ram(uint8_t* address);//read a byte from ram
        void push_to_stack(uint16_t data);//push data to the stack
        uint16_t pop_from_stack(); //get the top of stack
        void clear_registers();//clear registers
        uint8_t peek_next_byte(); //get the next byte of memory
        void print_reg_status();//print the status of the registers
        bool LH_reg_selector(uint8_t data);//select low or high part of a general purpose register
        uint8_t get_bits(int8_t low, int8_t high, uint8_t data);//get the bits acording to a range
        bool get_bit(uint8_t data, uint8_t num_bit);//get a bit from a byte
        ~CPU();
};

