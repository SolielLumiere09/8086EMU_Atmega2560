#include <MOV/MOV_0_OPCODE.h>


MOV_0_OPCODE::MOV_0_OPCODE(CPU *cpu){
    this->cpu = cpu;
}
void MOV_0_OPCODE::execute(uint8_t opcode)
{
    DATA_TRANSFER_FIELD fields;
    uint8_t fields_opcode = cpu->fetch();

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
                cpu->registers[fields.reg].data = cpu->registers[fields.rm].data;
            }
            else{
                //reg to rm
                cpu->registers[fields.rm].data = cpu->registers[fields.reg].data;
            }
        }
        else{
            //byte instruction
            if(fields.d){
                //rm to reg
                cpu->registers[fields.reg % 4].data_HL[cpu->LH_reg_selector(fields.reg)] = cpu->registers[fields.rm % 4].data_HL[cpu->LH_reg_selector(fields.rm)];
            
            }
            else{
                //reg to rm
                cpu->registers[fields.rm % 4].data_HL[cpu->LH_reg_selector(fields.rm)] = cpu->registers[fields.reg % 4].data_HL[cpu->LH_reg_selector(fields.reg)];

            }
        }

    }
    else if(fields.mod == 0b00 && fields.rm == 0b110){
         //if mod 00 and r/m = 110 then EA = disp-high;disp-low.

        uint16_t dispL = cpu->fetch(); //get Low disp
        uint16_t dispH = cpu->fetch(); //get High dip
        uint16_t disp = (cpu->segment_registers[DS_REG]*0x10) + ((dispH << 8) | dispL);
        cpu->write_or_read_ram(disp, &fields);

    }
    else if(fields.mod == 0b00){
        //if mod=00 then DISP=0*, disp-io and disp-high are absent
        uint16_t disp = cpu->get_effective_address(fields.rm, 0);
        cpu->write_or_read_ram(disp, &fields);
    }
    else if(fields.mod == 0b01){
        //if mod=01 then DISP= disp-low sign-extended to 16 bits,disp-highis absent
        int16_t dispL = (int16_t)cpu->fetch(); //get low disp
        uint16_t disp = cpu->get_effective_address(fields.rm, (uint16_t)dispL);

        cpu->write_or_read_ram(disp, &fields);
    }
    else if (fields.mod == 0b10){
        uint16_t dispL = cpu->fetch(); //get Low disp
        uint16_t dispH = cpu->fetch(); //get High dip
        uint16_t dispT = ((dispH << 8) | dispL);//complete disp
        uint16_t disp = cpu->get_effective_address(fields.rm, dispT);// effective address
        cpu->write_or_read_ram(disp, &fields);
    }
    
}
MOV_0_OPCODE::~MOV_0_OPCODE(){

}
