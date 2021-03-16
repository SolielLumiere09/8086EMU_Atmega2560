#pragma once
#include <Arduino.h>

#define and &&
#define or ||
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

#define MOV_0(opcode) ((opcode >> 2) & 0b00100010) //Register/Memory to/from Register
#define MOV_0_ID 0 

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

}MOV_0_FIELDS;

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
        void execute_MOV_0(MOV_0_FIELDS *fields);
        void update();
        ~CPU();
};

