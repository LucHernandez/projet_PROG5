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
    if(get_bit(ins,25)==0){ //I==0
        int offset = get_bits(ins,11,0);
        if(get_bit(ins,24)==1){ // P==1
            if(get_bit(ins,21)==1){ // W==1
                if(get_bit(ins,23)==1){ // U==1
                    result = RnVal + offset;
                    arm_write_register(p,(uint8_t)RnNum,result);
                    return result;
                }
                else{ // U==0
                    result = RnVal - offset;
                    arm_write_register(p,(uint8_t)RnNum,result);
                    return result;
                }
            }
            else{ // W==0
                if(get_bit(ins,23)==1){ // U==1
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
            if(get_bit(ins,21)==1){ // W==1
                if(get_bit(ins,23)==1){ // U==1
                    return UNDEFINED_INSTRUCTION;
                }
                else{ // U==0
                    return UNDEFINED_INSTRUCTION;
                }
            }
            else{ // W==0
                if(get_bit(ins,23)==1){ // U==1
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
    else{ // I==1
        int RmNum = get_bits(ins,3,0);
        int RnVal = arm_read_register(p,RmNum);
        
    }
}