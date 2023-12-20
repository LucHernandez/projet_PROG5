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
    return UNDEFINED_INSTRUCTION;
}

int arm_load_store_multiple(arm_core p, uint32_t ins) {
    return UNDEFINED_INSTRUCTION;
}

int arm_coprocessor_load_store(arm_core p, uint32_t ins) {
    /* Not implemented */
    return UNDEFINED_INSTRUCTION;
}

int verif_addr_mode(arm_core p,uint32_t ins){
    int RnNum = get_bits(ins,19,16);
    int RnVal = arm_read_register(p,RnNum);
    int result = 0;

    uint8_t i,pb,w,u;
    i = get_bit(ins,25);
    pb = get_bit(ins,24);
    w = get_bit(ins,21);
    u = get_bit(ins,23) ;
    if(i==0){ //Cas des immediate
        int offset = get_bits(ins,11,0);
        if(pb==1){ // P==1
            if(w==1){ // Immediate pre-index p464
                if(RnNum == 15){// Cas UNPREDICTABLE
                    return -1;
                }
                if(u==1){
                    result = RnVal + offset;
                    arm_write_register(p,(uint8_t)RnNum,result);
                    return result;
                }
                else{
                    result = RnVal - offset;
                    arm_write_register(p,(uint8_t)RnNum,result);
                    return result;
                }
            }
            else{ //Immediate offset p460
                if(RnNum == 15){
                    RnVal = RnVal + 8;  
                }
                if(u==1){ // U==1
                    result = RnVal + offset;
                    return result;
                }
                else{ // U==0
                    result = RnVal - offset;
                    return result;
                }  
            }
        }
        else{ //P==0
            if(w==1){ //cas des LDRBT,STRBT etc check bit W p468 et surtout 469
                if(u==1){ // U==1
                    return UNDEFINED_INSTRUCTION;
                }
                else{ // U==0
                    return UNDEFINED_INSTRUCTION;
                }
            }
            else{ // Immediate post-index p464
                if(RnNum == 15){ // Cas UNPREDICTABLE
                    return -1;
                }
                if(u==1){ // U==1
                    result = RnVal + offset;
                    arm_write_register(p,(uint8_t)RnNum,result);
                    return RnVal;
                }
                else{ // U==0
                    result = RnVal - offset;
                    arm_write_register(p,(uint8_t)RnNum,result);
                    return RnVal;
                }
            }

        }
    }
    else{ // Cas des registres
        int RmNum = get_bits(ins,3,0);
        int RmVal = arm_read_register(p,RmNum);
        int Shift = get_bits(ins,6,5);
        int Shift_imm = get_bits(ins,11,7);
        int index = 0;
        if(pb==1){
            if(w==1){
                if(get_bits(ins,11,4)==0){ //Reg pre-indexed p465
                    if((RmNum == 15) || (RnNum == 15)){ // Cas UNPREDICTABLE
                        return -1;
                    }
                    if(u==1){
                        result = RnVal + RmVal;
                        arm_write_register(p,(uint8_t)RnNum,result);
                        return result;
                    }
                    else{
                        result = RnVal - RmVal;
                        arm_write_register(p,(uint8_t)RnNum,result);
                        return result;
                    }
                }
                else{ //Scaled Reg pre-indexed p466
                    if((RmNum == 15) || (RnNum == 15)){ // Cas UNPREDICTABLE
                        return -1;
                    }
                    index = Shift_case(p,Shift,RmVal,Shift_imm);
                    if(u==1){
                        result = RnVal + index;
                        arm_write_register(p,(uint8_t)RnNum,result);
                        return result;
                    }
                    else{
                        result = RnVal - index;
                        arm_write_register(p,(uint8_t)RnNum,result);
                        return result;
                    }
                }
            }
            else{
                if(get_bits(ins,11,4)==0){ //Reg offset p461
                    if(RnNum == 15){
                        RnVal = RnVal + 8;;
                    }
                    if(RmNum == 15){ // Cas UNPREDICTABLE
                        return -1;
                    }
                    if(u==1){
                        result = RnVal + RmVal;
                        return result;
                    }
                    else{
                        result = RnVal - RmVal;
                        return result;
                    }
                }
                else{ //Scaled Reg offset p462
                    if(RnNum == 15){
                        RnVal = RnVal + 8;;
                    }
                    if(RmNum == 15){ // Cas UNPREDICTABLE
                        return -1;
                    }
                    index = Shift_case(p,Shift,RmVal,Shift_imm);
                    if(u==1){
                        result = RnVal + index;
                        return result;
                    }
                    else{
                        result = RnVal - index;
                        return result;
                    }
                }
            }
        }
        else{
            if(w==1){ //cas des LDRBT,STRBT etc check bit W p470 ou 471
                if(u==1){ // U==1
                    return UNDEFINED_INSTRUCTION;
                }
                else{ // U==0
                    return UNDEFINED_INSTRUCTION;
                }
            }
            else{
                if(get_bits(ins,11,4)==0){ //Reg post-indexed p470
                    if((RmNum == 15) || (RnNum == 15)){ // Cas UNPREDICTABLE
                        return -1;
                    }
                    if(u==1){
                        result = RnVal + RmVal;
                        arm_write_register(p,(uint8_t)RnNum,result);
                        return RnVal;
                    }
                    else{
                        result = RnVal - RmVal;
                        arm_write_register(p,(uint8_t)RnNum,result);
                        return RnVal;
                    }
                }
                else{ //Scaled Reg post-indexed p471
                    if((RmNum == 15) || (RnNum == 15)){ // Cas UNPREDICTABLE
                        return -1;
                    }
                    index = Shift_case(p,Shift,RmVal,Shift_imm);
                    if(u==1){
                        result = RnVal + index;
                        arm_write_register(p,(uint8_t)RnNum,result);
                        return RnVal;
                    }
                    else{
                        result = RnVal - index;
                        arm_write_register(p,(uint8_t)RnNum,result);
                        return RnVal;
                    }
                }
            }

        }
    }
}


int Shift_case(arm_core p,int Shift,int RmVal,int Shift_imm){
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
                index = (get_bit(arm_read_cpsr(p),29)<<31) || RmVal >> 1;
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


int arm_load_store_STR(arm_core p,uint32_t ins){
    return -1;
}

int arm_load_store_STRB(arm_core p,uint32_t ins){
    return -1;
}

int arm_load_store_STRH(arm_core p,uint32_t ins){
    return -1;
}

int arm_load_store_STM(arm_core p,uint32_t ins){
    return -1;
}

int arm_load_store_LDR(arm_core p,uint32_t ins){
    return -1;
}

int arm_load_store_LDRB(arm_core p,uint32_t ins){
    return -1;
}

int arm_load_store_LDRH(arm_core p,uint32_t ins){
    return -1;
}

int arm_load_store_LDM(arm_core p,uint32_t ins){
    return -1;
}
