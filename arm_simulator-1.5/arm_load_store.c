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

int STR(arm_core p, uint32_t ins){
    return UNDEFINED_INSTRUCTION;
}

int STRB(arm_core p, uint32_t ins){
    return UNDEFINED_INSTRUCTION;
}

int STRH(arm_core p, uint32_t ins){
    return UNDEFINED_INSTRUCTION;
}

int STM(arm_core p, uint32_t ins){
    return UNDEFINED_INSTRUCTION;
}

int LDR(arm_core p, uint32_t ins){
    return UNDEFINED_INSTRUCTION;
}

int LDRB(arm_core p, uint32_t ins){
    return UNDEFINED_INSTRUCTION;
}

int LDRH(arm_core p, uint32_t ins){
    return UNDEFINED_INSTRUCTION;
}

int LDM(arm_core p, uint32_t ins){
    return UNDEFINED_INSTRUCTION;
}

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
                err_instru = STRH(p, ins);
                return err_instru;
                }
                else{ //load hexa
                    err_instru = LDRH(p, ins);
                    return err_instru;
                }
            }
            return UNDEFINED_INSTRUCTION;
        case 0b010:
            if(bit_20 == 0){ //store
                if(bit_22 == 0){ //store word
                    err_instru = STR(p, ins);
                    return err_instru;
                }
                else{ //store byte
                    err_instru = STRB(p, ins);
                    return err_instru;
                }
            }
            else{ //load
                if(bit_22 == 0){ //load word
                    err_instru = LDR(p, ins);
                    return err_instru;
                }
                else{ //load byte
                    err_instru = LDRB(p, ins);
                    return err_instru;
                }
            }
        case 0b011:
            if(bit_20 == 0){ //store
                if(bit_22 == 0){ //store word
                    err_instru = STR(p, ins);
                    return err_instru;
                }
                else{ //store byte
                    err_instru = STRB(p, ins);
                    return err_instru;
                }
            }
            else{ //load
                if(bit_22 == 0){ //load word
                    err_instru = LDR(p, ins);
                    return err_instru;
                }
                else{ //load byte
                    err_instru = LDRB(p, ins);
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
        err_instru = STM(p, ins);
        return err_instru;
    }
    else{
        err_instru = LDM(p, ins); //load multiple
        return err_instru;
    }
}

int arm_coprocessor_load_store(arm_core p, uint32_t ins) {
    /* Not implemented */
    return UNDEFINED_INSTRUCTION;
}
