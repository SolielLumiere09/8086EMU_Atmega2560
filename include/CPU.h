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


#define MOV_0(opcode) ((opcode & 0b11111100) == 0b10001000) //Register/Memory to/from Register
#define MOV_0_ID 0 

#define MOV_1(opcode, reg) ((opcode & 0b11111110) == 0b11000110) && ((((~reg) >> 3) & 7))//Inmediate to Register/Memory
#define MOV_1_ID 1

#define MOV_2(opcode) ((opcode & 0b11110000) == 0b10110000) //Immediate to Register
#define MOV_2_ID 2

#define MOV_3(opcode) ((opcode & 0b11111110) == 0b10100000) ////Memory to Accumulator
#define MOV_3_ID 3

#define MOV_4(opcode) ((opcode & 0b11111110) == 0b10100010) //Accumulator to Memory
#define MOV_4_ID 4



typedef union 
{
    uint16_t data;
    uint8_t data_HL[2];
    
}REG;


typedef struct {
    unsigned CF : 1;
    unsigned PF : 1;
    unsigned AF : 1;
    unsigned ZF : 1;
    
}STATUS_REG; //flags of the status register

class MOV_0_OPCODE;
class MOV_1_OPCODE;
class MOV_2_OPCODE;
class MOV_3_OPCODE;
class MOV_4_OPCODE;

class CPU{
    private:
        MOV_0_OPCODE *mov_0;//Register/Memory to/from Register
        MOV_1_OPCODE *mov_1;//Inmediate to Register/Memory
        MOV_2_OPCODE *mov_2;//Immediate to Register
        MOV_3_OPCODE *mov_3;//Memory to Accumulator
        MOV_4_OPCODE *mov_4;//Accumulator to Memory


    public:
        uint16_t PC = 0; // program counter
        STATUS_REG sreg = {0}; //status register
        REG registers[TOTAL_REGS];//all reguisters
        uint16_t segment_registers[TOTAL_REGS];//all segment registers
        

        CPU();
        uint8_t fetch();//fetch instruction from memory
        uint8_t decode(uint8_t opcode);//decode the instruction
        
        uint16_t get_effective_address(uint8_t rm, uint16_t disp); //return the effective address acording to datasheet
        void write_to_ram(uint16_t* address, uint16_t data);//write a word into the ram
        void write_to_ram(uint8_t* address, uint8_t data);//wrrite a byte into the ram
        uint16_t read_from_ram(uint16_t* address);//read a word from ram
        uint8_t read_from_ram(uint8_t* address);//read a byte from ram
        void clear_registers();//clear registers
        void update();//update the cpu
        void print_reg_status();//print the status of the registers
        bool LH_reg_selector(uint8_t data);//select low or high part of a general purpose register
        uint8_t get_bits(int8_t low, int8_t high, uint8_t data);//get the bits acording to a range
        bool get_bit(uint8_t data, uint8_t num_bit);//get a bit from a byte
        ~CPU();
};

