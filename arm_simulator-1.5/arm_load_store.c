/*
Armator - simulateur de jeu d'instruction ARMv5T � but p�dagogique
Copyright (C) 2011 Guillaume Huard
Ce programme est libre, vous pouvez le redistribuer et/ou le modifier selon les
termes de la Licence Publique G�n�rale GNU publi�e par la Free Software
Foundation (version 2 ou bien toute autre version ult�rieure choisie par vous).

Ce programme est distribu� car potentiellement utile, mais SANS AUCUNE
GARANTIE, ni explicite ni implicite, y compris les garanties de
commercialisation ou d'adaptation dans un but sp�cifique. Reportez-vous � la
Licence Publique G�n�rale GNU pour plus de d�tails.

Vous devez avoir re�u une copie de la Licence Publique G�n�rale GNU en m�me
temps que ce programme ; si ce n'est pas le cas, �crivez � la Free Software
Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA 02111-1307,
�tats-Unis.

Contact: Guillaume.Huard@imag.fr
	 B�timent IMAG
	 700 avenue centrale, domaine universitaire
	 38401 Saint Martin d'H�res
*/
#include "arm_load_store.h"
#include "arm_exception.h"
#include "arm_constants.h"
#include "util.h"
#include "debug.h"


int arm_load_store(arm_core p, uint32_t ins) {
    int bits_27_25 = get_bits(ins, 27,25);
    int bit_22 = get_bit(ins, 22);
    int bit_20 = get_bit(ins, 20);
    int bits_7_4 = get_bits(ins, 7, 4);

    int err_instru;
    switch (bits_27_25)
    {
    case 0b000:
        if (bits_7_4 == 0b1011)
        { // load/store hexa
            if (bit_20 == 0)
            { // store hexa
                err_instru = arm_load_store_STRH(p, ins);
                return err_instru;
                }
                else{ //load hexa
                    err_instru = arm_load_store_LDRH(p, ins);
                    return err_instru;
                }
            }
            return UNDEFINED_INSTRUCTION;
        case 0b010:
            if(bit_20 == 0){ //store
                if(bit_22 == 0){ //store word
                    err_instru = arm_load_store_STR(p, ins);
                    return err_instru;
                }
                else{ //store byte
                    err_instru = arm_load_store_STRB(p, ins);
                    return err_instru;
                }
            }
            else{ //load
                if(bit_22 == 0){ //load word
                    err_instru = arm_load_store_LDR(p, ins);
                    return err_instru;
                }
                else{ //load byte
                    err_instru = arm_load_store_LDRB(p, ins);
                    return err_instru;
                }
            }
        case 0b011:
            if(bit_20 == 0){ //store
                if(bit_22 == 0){ //store word
                    err_instru = arm_load_store_STR(p, ins);
                    return err_instru;
                }
                else{ //store byte
                    err_instru = arm_load_store_STRB(p, ins);
                    return err_instru;
                }
            }
            else{ //load
                if(bit_22 == 0){ //load word
                    err_instru = arm_load_store_LDR(p, ins);
                    return err_instru;
                }
                else{ //load byte
                    err_instru = arm_load_store_LDRB(p, ins);
                    return err_instru;
                }
            }
        default:
            return UNDEFINED_INSTRUCTION;
        }
}

int arm_load_store_multiple(arm_core p, uint32_t ins) {
    int bit_20 = get_bit(ins, 20);

    int err_instru;
    if (bit_20 == 0){ //store multiple
        err_instru = arm_load_store_STM(p, ins);
        return err_instru;
    }
    else{
        err_instru = arm_load_store_LDM(p, ins); //load multiple
        return err_instru;
    }
}

int arm_coprocessor_load_store(arm_core p, uint32_t ins) {
    /* Not implemented */
    return UNDEFINED_INSTRUCTION;
}


/*FONCTION POUR LES ADDR_MODE*/

uint8_t addr_mode_WB(arm_core p,uint32_t ins,uint32_t *addr){
    uint8_t RnNum = get_bits(ins,19,16);
    uint32_t RnVal = arm_read_register(p,RnNum);
    uint32_t result = 0;

    uint8_t i,pb,w,u;
    i = get_bit(ins,25);
    pb = get_bit(ins,24);
    w = get_bit(ins,21);
    u = get_bit(ins,23) ;
    if(i==0){ //Cas des immediate
        int offset = get_bits(ins,11,0);
        if(pb==1){ // P==1
            if(w==1){ // Immediate pre-index p464 W=1,P=1,I=0
                if(RnNum == 15){// Cas UNPREDICTABLE
                    return DATA_ABORT;
                }
                if(u==1){
                    result = RnVal + offset;
                    arm_write_register(p,RnNum,result);
                    *addr = result;
                    return 0;
                }
                else{
                    result = RnVal - offset;
                    arm_write_register(p,RnNum,result);
                    *addr = result;
                    return 0;
                }
            }
            else{ //Immediate offset p460 W=0,P=1,I=0
                if(RnNum == 15){
                    RnVal = RnVal + 8;  
                }
                if(u==1){ // U==1
                    result = RnVal + offset;
                    *addr = result;
                    return 0;
                }
                else{ // U==0
                    result = RnVal - offset;
                    *addr = result;
                    return 0;
                }  
            }
        }
        else{ //P==0
            if(w==1){ //Cas des LDRBT,STRBT etc (pas a faire pour le moment) check bit W p468 et surtout 469 W=0,P=0,I=0
                if(u==1){ // U==1
                    return UNDEFINED_INSTRUCTION;
                }
                else{ // U==0
                    return UNDEFINED_INSTRUCTION;
                }
            }
            else{ // Immediate post-index p464 W=0,P=1,I=0
                if(RnNum == 15){ // Cas UNPREDICTABLE
                    return DATA_ABORT;
                }
                if(u==1){ // U==1
                    result = RnVal + offset;
                    arm_write_register(p,RnNum,result);
                    *addr = RnVal;
                    return 0;
                }
                else{ // U==0
                    result = RnVal - offset;
                    arm_write_register(p,RnNum,result);
                    *addr = RnVal;
                    return 0;
                }
            }

        }
    }
    else{ // Cas des registres
        uint8_t RmNum = get_bits(ins,3,0);
        uint32_t RmVal = arm_read_register(p,RmNum);
        int Shift = get_bits(ins,6,5);
        int Shift_imm = get_bits(ins,11,7);
        int index = 0;
        if(pb==1){
            if(w==1){
                if(get_bits(ins,11,4)==0){ //Reg pre-indexed p465 W=1,P=1,I=1 et les bits 11 à 4 == 0
                    if((RmNum == 15) || (RnNum == 15)){ // Cas UNPREDICTABLE
                        return DATA_ABORT;
                    }
                    if(u==1){
                        result = RnVal + RmVal;
                        arm_write_register(p,RnNum,result);
                        *addr = result;
                        return 0;
                    }
                    else{
                        result = RnVal - RmVal;
                        arm_write_register(p,RnNum,result);
                        *addr = result;
                        return 0;
                    }
                }
                else{ //Scaled Reg pre-indexed p466 W=1,P=1,I=1 et les bits 11 à 4 != 0
                    if((RmNum == 15) || (RnNum == 15)){ // Cas UNPREDICTABLE
                        return DATA_ABORT;
                    }
                    index = Shift_case(p,Shift,RmVal,Shift_imm);
                    if(u==1){
                        result = RnVal + index;
                        arm_write_register(p,RnNum,result);
                        *addr = result;
                        return 0;
                    }
                    else{
                        result = RnVal - index;
                        arm_write_register(p,RnNum,result);
                        *addr = result;
                        return 0;
                    }
                }
            }
            else{
                if(get_bits(ins,11,4)==0){ //Reg offset p461 W=0,P=1,I=1 et les bits 11 à 4 == 0
                    if(RnNum == 15){
                        RnVal = RnVal + 8;;
                    }
                    if(RmNum == 15){ // Cas UNPREDICTABLE
                        return DATA_ABORT;
                    }
                    if(u==1){
                        result = RnVal + RmVal;
                        *addr = result;
                        return 0;
                    }
                    else{
                        result = RnVal - RmVal;
                        *addr = result;
                        return 0;
                    }
                }
                else{ //Scaled Reg offset p462 W=0,P=1,I=1 et les bits 11 à 4 != 0
                    if(RnNum == 15){
                        RnVal = RnVal + 8;
                    }
                    if(RmNum == 15){ // Cas UNPREDICTABLE
                        return DATA_ABORT;
                    }
                    index = Shift_case(p,Shift,RmVal,Shift_imm);
                    if(u==1){
                        result = RnVal + index;
                        *addr = result;
                        return 0;
                    }
                    else{
                        result = RnVal - index;
                        *addr = result;
                        return 0;
                    }
                }
            }
        }
        else{
            if(w==1){ //Cas des LDRBT,STRBT etc (pas a faire pour le moment) check bit W p470 ou 471 W=1,P=0,I=1
                if(u==1){ // U==1
                    return UNDEFINED_INSTRUCTION;
                }
                else{ // U==0
                    return UNDEFINED_INSTRUCTION;
                }
            }
            else{
                if(get_bits(ins,11,4)==0){ //Reg post-indexed p470 W=0,P=0,I=1 et les bits 11 à 4 == 0
                    if((RmNum == 15) || (RnNum == 15)){ // Cas UNPREDICTABLE
                        return DATA_ABORT;
                    }
                    if(u==1){
                        result = RnVal + RmVal;
                        arm_write_register(p,RnNum,result);
                        *addr = RnVal;
                        return 0;
                    }
                    else{
                        result = RnVal - RmVal;
                        arm_write_register(p,RnNum,result);
                        *addr = RnVal;
                        return 0;
                    }
                }
                else{ //Scaled Reg post-indexed p471 W=0,P=0,I=1 et les bits 11 à 4 != 0
                    if((RmNum == 15) || (RnNum == 15)){ // Cas UNPREDICTABLE
                        return DATA_ABORT;
                    }
                    index = Shift_case(p,Shift,RmVal,Shift_imm);
                    if(u==1){
                        result = RnVal + index;
                        arm_write_register(p,RnNum,result);
                        *addr = RnVal;
                        return 0;
                    }
                    else{
                        result = RnVal - index;
                        arm_write_register(p,RnNum,result);
                        *addr = RnVal;
                        return 0;
                    }
                }
            }

        }
    }
}


uint8_t addr_mode_H(arm_core p,uint32_t ins,uint32_t *addr){
    uint8_t i,pb,w,u;
    i = get_bit(ins,22);
    pb = get_bit(ins,24);
    w = get_bit(ins,21);
    u = get_bit(ins,23);
    uint8_t RnNum = get_bits(ins,19,16);
    int32_t RnVal = arm_read_register(p,RnNum);
    int32_t result = 0;
    if(i==1){
        uint8_t immedH = get_bits(ins,11,8);
        uint8_t ImmedL = get_bits(ins,3,0);
        uint8_t offset = (immedH << 4)|ImmedL;;
        if(pb==0){ 
            if(w==0){ // Immediate Post-Indexed p479
                if(RnNum == 15){
                    return DATA_ABORT;
                }
                if(u==1){
                    result = RnVal + offset;
                    arm_write_register(p,RnNum,result);
                    *addr = RnVal;
                    return 0;
                }
                else{
                    result = RnVal - offset;
                    arm_write_register(p,RnNum,result);
                    *addr = RnVal;
                    return 0;
                }
            }
            else{
                return DATA_ABORT;
            }
        }
        else{
            if(w==0){// Immediate Offset p475
                if(RnNum == 15){
                    RnVal = RnVal + 8;
                }
                if(u==0){
                    result = RnVal + offset;
                    *addr = result;
                    return 0;
                }
                else{
                    result = RnVal - offset;
                    *addr = result;
                    return 0;
                }
            }
            else{// Immediate Pre-Indexed p477
                if(RnNum == 15 ){
                    return DATA_ABORT;
                }
                if(u==1){
                    result = RnVal + offset;
                    arm_write_register(p,RnNum,result);
                    *addr = result;
                    return 0;
                }
                else{
                    result = RnVal - offset;
                    arm_write_register(p,RnNum,result);
                    *addr = result;
                    return 0;
                }
            }
        }
    }
    else{

        uint8_t RmNum = get_bits(ins,3,0);
        uint32_t RmVal = arm_read_register(p,RmNum);

        if(pb==0){
            if(w==0){// Register Post-Indexed p480
                if((RmNum == 15) || (RnNum == 15)){ // Cas UNPREDICTABLE
                    return DATA_ABORT;
                }
                if(RmNum==RnNum){
                    return DATA_ABORT;
                }
                if(u==1){
                    result = RnVal + RmVal;
                    arm_write_register(p,RnNum,result);
                    *addr = RnVal;
                    return 0;
                }
                else{
                    result = RnVal - RmVal;
                    arm_write_register(p,RnNum,result);
                    *addr = RnVal;
                    return 0;
                }
            }
            else{
                return DATA_ABORT;
            }
        }
        else{
            if(w==0){// Reg Offset p476
                if(RnNum == 15){
                    RnVal = RnVal + 8;;
                }
                if(RmNum == 15){ // Cas UNPREDICTABLE
                    return DATA_ABORT;
                }
                if(u==1){
                    result = RnVal + RmVal;
                    *addr = result;
                    return 0;
                }
                else{
                    result = RnVal - RmVal;
                    *addr = result;
                    return 0;
                }
            }
            else{// Reg Pre-Indexed p478
                if((RmNum == 15) || (RnNum == 15)){ // Cas UNPREDICTABLE
                    return DATA_ABORT;
                }
                if(RmNum==RnNum){
                    return DATA_ABORT;
                }
                if(u==1){
                    result = RnVal + RmVal;
                    arm_write_register(p,RnNum,result);
                    *addr = result;
                    return 0;
                }
                else{
                    result = RnVal - RmVal;
                    arm_write_register(p,RnNum,result);
                    *addr = result;
                    return 0;
                }

            }

        }
    }   
}

uint8_t addr_mode_M(arm_core p,uint32_t ins,uint32_t *start_address,uint32_t *end_address){
    if (get_bit(ins,22) && !arm_current_mode_has_spsr(p)){
        return DATA_ABORT;
    }

    uint8_t bit_P_U = get_bits(ins,24,23);
    uint8_t RnNum = get_bits(ins,19,16);
    int32_t RnVal = arm_read_register(p,RnNum);
    uint8_t bit_w = get_bit(ins,21);
    switch (bit_P_U){
        case 0b01:
            *start_address = RnVal;
            *end_address = RnVal + (Number_Of_Set_Bits_In(ins) * 4) - 4;
            if (bit_w){
                arm_write_register(p,RnNum,RnVal + Number_Of_Set_Bits_In(ins) * 4 );
            }
            break;
        case 0b11:
            *start_address = RnVal + 4;
            *end_address = RnVal + (Number_Of_Set_Bits_In(ins) * 4);
            if (bit_w){
                arm_write_register(p,RnNum,RnVal + Number_Of_Set_Bits_In(ins) * 4 );
            }
            break;
        case 0b00:
            *start_address = RnVal - (Number_Of_Set_Bits_In(ins) * 4) + 4;
            *end_address = RnVal ;
            if (bit_w){
                arm_write_register(p,RnNum,RnVal - Number_Of_Set_Bits_In(ins) * 4 );
            }
            break;
        case 0b10:
            *start_address = RnVal - (Number_Of_Set_Bits_In(ins) * 4);
            *end_address = RnVal - 4;
            if (bit_w){
                arm_write_register(p,RnNum,RnVal - Number_Of_Set_Bits_In(ins) * 4 );
            }
            break;
        default:
            return DATA_ABORT;
    }
    return 0;
}

/**/

int Shift_case(arm_core p,int Shift,int32_t RmVal,int Shift_imm){
    int index = 0;
    switch(Shift){
        case 0b00:
            index = RmVal << Shift_imm;
            break;
        case 0b01:
            if(Shift_imm == 0){
                index = 0;
                break;
            }
            else{
                index = RmVal >> Shift_imm;
                break;
            }
        case 0b10:
            if(Shift_imm == 0){
                if(get_bit(RmVal,31)== 1){
                    index = 0xFFFFFFFF;
                    break;
                }
                else{
                    index = 0;
                    break;
                }
            }
            else{
                index = asr(RmVal,Shift_imm);
                break;
            }
        case 0b11:
            if(Shift_imm == 0){
                index = (get_bit(arm_read_cpsr(p),29)<<31) | RmVal >> 1;
                break;
            }
            else{
                index = ror(RmVal,Shift_imm);
                break;
            }
        default:
            printf("peu pas ton b 2 dan");
            break;
    }
    return index;
}

int Number_Of_Set_Bits_In(uint32_t ins){
    int i;
    int result = 0;
    for (i = 0; i <16;++i) result += get_bit(ins,i);
    return result;
}

/*FONCTION POUR LES INSTRUCTION D'ARM*/
int arm_load_store_STR(arm_core p,uint32_t ins){
    uint32_t addr =0;
    uint32_t value = arm_read_register(p,get_bits(ins,15,12));
    if(addr_mode_WB(p,ins,&addr)){
        return DATA_ABORT;
    }
    arm_write_word(p,addr,value);
    return 0;
}

int arm_load_store_STRB(arm_core p,uint32_t ins){
    uint32_t addr = 0;
    uint8_t value = arm_read_register(p,get_bits(ins,15,12));
    if(addr_mode_WB(p,ins,&addr)){
        return DATA_ABORT;
    }    
    arm_write_byte(p,addr,value);
    return 0;
}

int arm_load_store_STRH(arm_core p,uint32_t ins){
    uint32_t addr = 0;
    uint16_t value = arm_read_register(p,get_bits(ins,15,12));
    if(addr_mode_H(p,ins,&addr)){
        return DATA_ABORT;
    }
    if(get_bit(addr,0)==0){
        arm_write_half(p,addr,value);
        return 0;
    }
    else{
        return DATA_ABORT;
    }

}

int arm_load_store_STM(arm_core p,uint32_t ins){
    uint32_t start_address,end_address;
    uint32_t address;
    uint32_t value;

    if (addr_mode_M(p,ins,&start_address,&end_address)){
        return DATA_ABORT;
    }

    address = start_address;
    for(int i;i<15;i++){
            if(get_bit(ins,i)){
                value = arm_read_register(p,i);
                arm_write_word(p,address,value);
                address += 4;
            }
    }

    if (end_address != (address -4)) return DATA_ABORT;
    return 0;
}

int arm_load_store_LDR(arm_core p,uint32_t ins){
    uint32_t address;

    if(addr_mode_WB(p,ins,&address)){
        return DATA_ABORT;
    }

    uint32_t value;
    int data =ror(arm_read_word(p,address,&value),8*get_bits(address,1,0));

    if (get_bits(ins,15,12) == 15){
        arm_write_register(p,15,data & 0xFFFFFFFE);
        arm_write_cpsr(p,set_bits(arm_read_cpsr(p),5,5,get_bit(data,0)));
    }
    else{
        arm_write_register(p,get_bits(ins,15,12),data);
    }
    
    return 0;
}

int arm_load_store_LDRB(arm_core p,uint32_t ins){
    // Rd = Memory[address,1]
    uint32_t address = 0;

    if(addr_mode_WB(p,ins,&address)){
        return DATA_ABORT;
    }

    uint8_t value;

    arm_read_byte(p,address,&value);
    arm_write_register(p,get_bits(ins,15,12),value);
    return 0;
}

int arm_load_store_LDRH(arm_core p,uint32_t ins){
    uint32_t address;
    if(addr_mode_H(p,ins,&address)){
        return DATA_ABORT;
    }
    uint16_t data = 0;
    if get_bit(address,0){
        arm_read_half(p,address,&data);
    }
    arm_write_register(p,get_bits(ins,15,12),((uint32_t) 0 |(uint32_t) data));
    return 0;
}

int arm_load_store_LDM(arm_core p,uint32_t ins){
    uint32_t start_address,end_address;
    uint32_t address;

    if (addr_mode_M(p,ins,&start_address,&end_address)){
        return DATA_ABORT;
    }

    address = start_address;
    int i;
    uint32_t value;
    
    for (i = 0; i< 15; ++i){
        if (get_bit(ins,i)){
            arm_read_word(p,address,&value);
            arm_write_register(p,i,value);
            address +=4;
        }
    }

    if (get_bit(ins,15)){
        i = arm_read_word(p,address,&value);
        if (i == -1) return -1;
        arm_write_register(p,15, value & 0xFFFFFFFE);
        arm_write_cpsr(p,set_bits(arm_read_cpsr(p),5,5,get_bit(value,0)));

        address +=4;
    }

    if (end_address != (address -4)) return DATA_ABORT;
    return 0;
}
