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


#define d_field 1
#define w_field 0
#define rm_field 0
#define mod_field 6
#define reg_field 3
#define OFFSET 0x2200
#define ADD_TO_ADDR(address) (((uint16_t)address)+OFFSET)
#define LH_reg_r (fields->reg >> 2)
#define LH_rm_r (fields->rm >> 2)
#define LH_reg_v (fields.reg >> 2)
#define LH_rm_v (fields.rm >> 2)


#define MOV_0(opcode) ((opcode >> 2) == 0b00100010) //Register/Memory to/from Register
#define MOV_0_ID 0 

#define MOV_1(opcode, reg) ((opcode >> 1) == 0b01100011) && ((((~reg) >> 3) & 7))//Inmediate to Register/Memory
#define MOV_1_ID 1

typedef union 
{
    uint16_t data;
    uint8_t data_HL[2];
    
}REG;

typedef struct{
    unsigned d : 1; //d for "To reg" or 
    unsigned w : 1; //word or byte instruction
    unsigned mod: 2; //mod field
    unsigned reg : 3; //reg field
    unsigned rm : 3; // r/m field

}DATA_TRANSFER_FIELD;


typedef struct {
    unsigned CF : 1;
    unsigned PF : 1;
    unsigned AF : 1;
    unsigned ZF : 1;
    
}STATUS_REG;


class CPU{

    private:
        uint16_t PC = 0;
        STATUS_REG sreg = {0};
        REG registers[TOTAL_REGS];
        uint16_t segment_registers[TOTAL_REGS];
        
    public:
        CPU();
        uint8_t fetch();
        uint8_t decode(uint8_t opcode);
        
        uint16_t get_effective_address(uint8_t rm, uint16_t disp); 
        void write_to_ram(uint16_t* address, uint16_t data);
        void write_to_ram(uint8_t* address, uint8_t data);
        uint16_t read_from_ram(uint16_t* address);
        uint8_t read_from_ram(uint8_t* address);
        void write_or_read_ram(uint16_t disp, DATA_TRANSFER_FIELD *fields);
        void clear_registers();
        void update();
        void print_reg_status();

        void execute_MOV_0(uint8_t opcode);
        void execute_MOV_1(uint8_t opcode);
        
        ~CPU();
};

