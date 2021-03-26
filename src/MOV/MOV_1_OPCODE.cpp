#include <MOV/MOV_1_OPCODE.h>


MOV_1_OPCODE::MOV_1_OPCODE(){

}
void MOV_1_OPCODE::execute(uint8_t opcode){
    
    #define w_field 0
    #define rm_field 0
    #define mod_field 6

    MOV_1_FIELDS fields;
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
            uint16_t disp = CPU::cpu->get_word_disp(); //get the address
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
                        uint16_t disp = CPU::cpu-> get_effective_address(fields.rm, 0x0000); //get effective address
                        CPU::cpu->write_to_ram((uint16_t*)disp, data); // write data 

                    }
                    else{
                        //byte instruction
                        uint8_t data = CPU::cpu->get_byte();
                        uint16_t disp = CPU::cpu->get_effective_address(fields.rm, 0x0000);
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
MOV_1_OPCODE::~MOV_1_OPCODE(){
}
