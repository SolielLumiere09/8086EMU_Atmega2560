#include <MOV.h>

MOV::MOV(/* args */)
{
}

void MOV::execute_mov_register_or_memory_to_or_from_register(uint8_t opcode){
        #define d_field 1
    #define w_field 0
    #define rm_field 0
    #define mod_field 6
    #define reg_field 3

    mov_register_or_memory_to_or_from_register_fields fields;
    uint8_t fields_opcode = CPU::cpu->fetch();

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
                CPU::cpu->registers[fields.reg].data = CPU::cpu->registers[fields.rm].data;
            }
            else{
                //reg to rm
                CPU::cpu->registers[fields.rm].data = CPU::cpu->registers[fields.reg].data;
            }
        }
        else{
            //byte instruction
            if(fields.d){
                //rm to reg
               CPU:: cpu->registers[fields.reg % 4].data_HL[CPU::cpu->LH_reg_selector(fields.reg)] = CPU::cpu->registers[fields.rm % 4].data_HL[CPU::cpu->LH_reg_selector(fields.rm)];
            
            }
            else{
                //reg to rm
                CPU::cpu->registers[fields.rm % 4].data_HL[CPU::cpu->LH_reg_selector(fields.rm)] = CPU::cpu->registers[fields.reg % 4].data_HL[CPU::cpu->LH_reg_selector(fields.reg)];

            }
        }

    }
    else if(fields.mod == 0b00 && fields.rm == 0b110){
         //if mod 00 and r/m = 110 then EA = disp-high;disp-low.

        uint16_t disp = CPU::cpu->get_word_disp(DS_REG); //get the disp
        write_or_read_ram(disp, &fields);

    }
    else if(fields.mod == 0b00){

        
        //if mod=00 then DISP=0*, disp-io and disp-high are absent
        uint16_t disp = CPU::cpu->get_effective_address(fields.rm, 0);
        write_or_read_ram(disp, &fields);
    }
    else if(fields.mod == 0b01){
        //if mod=01 then DISP= disp-low sign-extended to 16 bits,disp-highis absent
        int16_t disp = CPU::cpu->get_word_signed_disp(); //get disp
        uint16_t EA = CPU::cpu->get_effective_address(fields.rm, (uint16_t)disp);

        write_or_read_ram(EA, &fields);
    }
    else if (fields.mod == 0b10){
        ////if mod=10 then DISP= disp-high:disp-low
        uint16_t disp = CPU::cpu->get_word_disp(); //get disp
        write_or_read_ram(CPU::cpu->get_effective_address(fields.rm, disp), &fields);
    }

    #undef d_field 
    #undef w_field 
    #undef rm_field 
    #undef mod_field 
    #undef reg_field 

}

void MOV::execute_inmediate_to_register_or_memory(uint8_t opcode){
    #define w_field 0
    #define rm_field 0
    #define mod_field 6

    inmediate_to_register_or_memory_fields fields;
    uint8_t fields_opcode = CPU::cpu->fetch();

    fields.w = opcode >> w_field;
    fields.mod = fields_opcode >> mod_field;
    fields.rm = fields_opcode >> rm_field;

    if(fields.mod == 0b11){
        //inmediate to register 
        if(fields.w){
            //word instruction
            uint16_t data = CPU::cpu->get_word(); //get data 
            CPU::cpu->registers[fields.rm].data = data; //write to register

        }
        else{
            //byte instruction
            uint8_t data = CPU::cpu->get_byte(); //get byte
            CPU::cpu->registers[fields.rm % 4].data_HL[CPU::cpu->LH_reg_selector(fields.rm)] = data; //write to register
        }
    }
    else if(fields.mod == 0b00 && fields.rm == 0b110){
        //inmediate to memory
        //except if mod = 00 and r/m = 110 then EA = disp-high;disp-low

        if(fields.w){
            //word instruction
            uint16_t disp = CPU::cpu->get_word_disp(DS_REG); //get the address
            uint16_t data = CPU::cpu->get_word(); //get word 
           
            CPU::cpu->write_to_ram((uint16_t*)disp, data);

        }
        else{
            //byte instruction
            uint16_t disp = CPU::cpu->get_word_disp(); //get the disp
            uint8_t data = CPU::cpu->get_byte(); // get data
            CPU::cpu->write_to_ram((uint8_t*)disp, data);

        }

    }
    else{

        switch(fields.mod){

        case 0b00:  //if mode = 00 then DISP = 0 ,disp-low and disp-high areabsent
                    if(fields.w){
                        //word instruction
                        uint16_t data = CPU::cpu->get_word(); //get data
                        uint16_t disp = CPU::cpu-> get_effective_address(fields.rm, 0); //get effective address
                        CPU::cpu->write_to_ram((uint16_t*)disp, data); // write data 

                    }
                    else{
                        //byte instruction
                        uint8_t data = CPU::cpu->get_byte();
                        uint16_t disp = CPU::cpu->get_effective_address(fields.rm, 0);
                        CPU::cpu->write_to_ram((uint8_t*)disp, data);
                    }
                    break;

        case 0b01:  
                    //if mode = 1 then DISP = disp-low sign-extended to 16 bits,disp-high is absent
                    if(fields.w){
                        //word instruction
                        int16_t disp = CPU::cpu->get_word_signed_disp();// get disp
                        uint16_t data = CPU::cpu->get_word(); //get data
                        CPU::cpu->write_to_ram((uint16_t*)CPU::cpu->get_effective_address(fields.rm, (uint16_t)disp), data); //write the value in ram

                    }  
                    else{
                        //byte instruction
                        int16_t disp = CPU::cpu->get_word_signed_disp();// get disp
                        uint8_t data = CPU::cpu->get_byte();//get data
                        CPU::cpu->write_to_ram((uint8_t*)CPU::cpu->get_effective_address(fields.rm, (uint16_t)disp), data);

                    }
                    break;

        case 0b10:  //if mod = 10 then DISP = disp-high;disp-low
                    if(fields.w){
                        //word instruction
                        uint16_t disp = CPU::cpu->get_word_disp();//get disp
                        uint16_t data = CPU::cpu->get_word();//get word 
                        CPU::cpu->write_to_ram((uint16_t*)CPU::cpu->get_effective_address(fields.rm, disp), data);
                    }
                    else{
                        //byte instruction
                        uint16_t disp = CPU::cpu->get_word_disp();//get disp
                        uint8_t data = CPU::cpu->get_byte(); //get the data
                        CPU::cpu->write_to_ram((uint8_t*)CPU::cpu->get_effective_address(fields.rm, disp), data);
                    }
                    break;
        }

    }

    #undef w_field 
    #undef rm_field 
    #undef mod_field 
}

void MOV::execute_inmediate_to_register(uint8_t opcode){
    #define w_field 3
    #define reg_fieldL 0
    #define reg_fieldH 2
    
    
    inmediate_to_register_fields fields;

    fields.w = CPU::cpu->get_bit(opcode, w_field);
    fields.reg = CPU::cpu->get_bits(reg_fieldL, reg_fieldH, opcode);

    if(fields.w){
        //word instruction
        uint16_t data = CPU::cpu->get_word();//get data 
        CPU::cpu->registers[fields.reg].data = data; //copy data 

    }
    else{
        //byte instruction
        uint8_t data = CPU::cpu->get_byte();
        CPU::cpu->registers[fields.reg % 4].data_HL[CPU::cpu->LH_reg_selector(fields.reg)] = data;
    }


    #undef w_field
    #undef reg_field

}

void MOV::execute_memory_to_accumulator(uint8_t opcode){
    #define w_field 0

    bool w = CPU::cpu->get_bit(opcode, w_field);
    uint16_t disp = CPU::cpu->get_word_disp(); //get disp

    if(w){
        //word instruction
        CPU::cpu->registers[AX_REG].data = CPU::cpu->read_from_ram((uint16_t*)disp); //read a word from ram 

    }
    else{
        //byte instruction
        CPU::cpu->registers[AX_REG].data_HL[LOW] = CPU::cpu->read_from_ram((uint8_t*)disp); //read a byte
    }


    #undef w_field
}

void MOV::execute_accumulator_to_memory(uint8_t opcode){
    #define w_bit 0
    
    bool w = CPU::cpu->get_bit(opcode, w_bit); // get the w bit
    uint16_t disp = CPU::cpu->get_word_disp(); //get disp

    if(w){
        //word instruction
        CPU::cpu->write_to_ram((uint16_t*)disp, CPU::cpu->registers[AX_REG].data);

    }
    else{
        //byte instruction
        CPU::cpu->write_to_ram((uint8_t*)disp, CPU::cpu->registers[AX_REG].data_HL[LOW]);
    }


    #undef w_bit

}

void MOV::write_or_read_ram(uint16_t disp, mov_register_or_memory_to_or_from_register_fields *fields){
    if(fields->d == 0)
    {
        //write data to reg
        if(fields->w)
        {
            //write 16 bits reg
            CPU::cpu->write_to_ram((uint16_t*)disp, CPU::cpu->registers[fields->reg].data);
        }
        else
        {
            //write 8 bits reg
            CPU::cpu->write_to_ram((uint8_t*)disp, CPU::cpu->registers[fields->reg % 4].data_HL[CPU::cpu->LH_reg_selector(fields->reg)]);
        }
    }
    else
    {
        //read data to reg
        if(fields->w)
        {
            //read a word 
            CPU::cpu->registers[fields->reg % 4].data = CPU::cpu->read_from_ram((uint16_t*)disp);

        }
        else
        {
            //read a byte
            CPU::cpu->registers[fields->reg % 4].data_HL[CPU::cpu->LH_reg_selector(fields->reg)] = CPU::cpu->read_from_ram((uint8_t*)disp);
        }
    }
}
MOV::~MOV()
{
}
