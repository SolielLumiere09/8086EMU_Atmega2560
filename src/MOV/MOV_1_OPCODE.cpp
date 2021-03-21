#include <MOV/MOV_1_OPCODE.h>


MOV_1_OPCODE::MOV_1_OPCODE(CPU *cpu){
    this->cpu = cpu;
}
void MOV_1_OPCODE::execute(uint8_t opcode){
    DATA_TRANSFER_FIELD fields;
    uint8_t fields_opcode = cpu->fetch();

    fields.w = opcode >> w_field;
    fields.mod = fields_opcode >> mod_field;
    fields.rm = fields_opcode >> rm_field;

    if(fields.mod == 0b11){
        //inmediate to register 
        if(fields.w){
            //word instruction
            uint16_t dataL = cpu->fetch();
            uint16_t dataH = cpu->fetch();
            uint16_t data = (dataH << 8) | dataL;

            cpu->registers[fields.rm].data = data;

        }
        else{
            //byte instruction
            uint8_t data = cpu->fetch();

            cpu->registers[fields.rm % 4].data_HL[cpu->LH_reg_selector(fields.rm)] = data;
        }
    }
    else if(fields.mod == 0b00 && fields.rm == 0b110){
        //inmediate to memory
        //except if mod = 00 and r/m = 110 then EA = disp-high;disp-low

        if(fields.w){
            //word instruction
            uint16_t disp = cpu->fetch(); //get disp
            disp |= ((uint16_t)cpu->fetch() << 8);

            uint16_t data = cpu->fetch(); // get data
            data |= ((uint16_t)cpu->fetch() << 8);
           
            cpu->write_to_ram((uint16_t*)disp, data);

        }
        else{
            //byte instruction
            uint16_t disp = cpu->fetch(); // get disp
            disp |= ((uint16_t)cpu->fetch() << 8);

            uint8_t data = cpu->fetch(); // get data
            
            cpu->write_to_ram((uint8_t*)disp, data);

        }

    }
    else{

        switch(fields.mod){

        case 0b00:  //if mode = 00 then DISP = 0 ,disp-low and disp-high areabsent
                    if(fields.w){
                        //word instruction
                        uint16_t data = cpu->fetch(); //get data
                        data |= ((uint16_t)cpu->fetch() << 8);
                        Serial.println(data, 16);
                        uint16_t disp =cpu-> get_effective_address(fields.rm, 0x0000);
                        cpu->write_to_ram((uint16_t*)disp, data); // write data 

                    }
                    else{
                        //byte instruction
                        uint8_t data = cpu->fetch();
                        uint16_t disp = cpu->get_effective_address(fields.rm, 0x0000);
                        cpu->write_to_ram((uint8_t*)disp, data);
                    }
                    break;

            case 0b01:  
                        //if mode = 1 then DISP = disp-low sign-extended to 16 bits,disp-high is absent
                        if(fields.w){
                            //word instruction
                            int16_t disp = (int16_t)cpu->fetch();// get disp
                            
                            uint16_t data = cpu->fetch(); //get data L
                            data |= ((uint16_t)cpu->fetch() << 8); //get data H

                            cpu->write_to_ram((uint16_t*)cpu->get_effective_address(fields.rm, (uint16_t)disp), data); //write the value in ram

                        }  
                        else{
                            //byte instruction
                            int16_t disp = (int16_t)cpu->fetch();// get disp
                            uint8_t data = cpu->fetch();//get data
                            cpu->write_to_ram((uint8_t*)cpu->get_effective_address(fields.rm, (uint16_t)disp), data);

                        }
                        break;

            case 0b10:  //if mod = 10 then DISP = disp-high;disp-low
                        if(fields.w){
                            //word instruction
                            uint16_t disp = (uint16_t)cpu->fetch(); //get dispL
                            disp |= ((uint16_t)cpu->fetch() << 8); //get dispH

                            uint16_t data = (uint16_t)cpu->fetch(); //get dataL
                            data |= ((uint16_t)cpu->fetch() << 8); //get dataH

                            cpu->write_to_ram((uint16_t*)cpu->get_effective_address(fields.rm, disp), data);

                        }
                        else{
                            //byte instruction
                            uint16_t disp = (uint16_t)cpu->fetch(); //get dispL
                            disp |= ((uint16_t)cpu->fetch() << 8); //get dispH

                            uint8_t data = cpu->fetch(); //get the data

                            cpu->write_to_ram((uint8_t*)cpu->get_effective_address(fields.rm, disp), data);
                        }
        }

    }

}
MOV_1_OPCODE::~MOV_1_OPCODE(){
}
