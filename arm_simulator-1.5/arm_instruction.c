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
#include "arm_instruction.h"
#include "arm_exception.h"
#include "arm_data_processing.h"
#include "arm_load_store.h"
#include "arm_branch_other.h"
#include "arm_constants.h"
#include "util.h"

int arm_instruction_condition(arm_core p, uint32_t instruction){
    uint32_t reg_cpsr = arm_read_cpsr(p);
    int z = get_bit(reg_cpsr, Z); //recuperation des bits de flags dans CPSR
    int n = get_bit(reg_cpsr, N);
    int c = get_bit(reg_cpsr, C);
    int v = get_bit(reg_cpsr, V);
    switch(get_bits(instruction, 31, 28)){ //recuperation des bits de conditions de l'instruction
        case 0b0000: //case equal
            if(z == 1){
                return 0;
            }
            return -1;
        case 0b0001: //case not equal
            if(z == 0){
                return 0;
            }
            return -1;
        case 0b0010: //case carry set / unsigned higher or same
            if(c == 1){
                return;
            }
            return -1;
        case 0b0011: //case carry clear/ unsigned lower
            if(c == 0){
                return 0;
            }
            return -1;
        case 0b0100: //case minus/ negative
            if(n == 1){
                return 0;
            }
            return -1;
        case 0b0101: //case plus/ positive or zero
            if(n == 0){
                return 0;
            }
            return -1;
        case 0b0110: //case overflow
            if(v == 1){
                return 0;
            }
            return -1;
        case 0b0111: //case no overflow
            if(v == 0){
                return 0;
            }
            return -1;
        case 0b1000: //case unsigned higher
            if(c == 1 && z == 0){
                return 0;
            }
            return -1;
        case 0b1001: //case unsigned lower or same
            if(c == 0 || z == 1){
                return 0;
            }
            return -1;
        case 0b1010: //case signed greater than or equal
            if(n == v){
                return 0;
            }
            return -1;
        case 0b1011: //case signed less than
            if(n != v){
                return 0;
            }
            return -1;
        case 0b1100: //case signed greater than
        if(z == 0 && n == v){
            return 0;
        }
        return -1;
        case 0b1101: //case signed less than or equal
        if(z == 1 || n != v){
            return 0;
        }
        return -1;
        case 0b1110: //case always
            return 0;
        case 0b1111: //case unpredictable or an instruction extension
            return 1;
        default:
            return -1;
        }
}

int arm_instruction_type_decode(arm_core p, uint32_t instruction){
    return UNDEFINED_INSTRUCTION;
}

static int arm_execute_instruction(arm_core p) {
    uint32_t instruction;
    int res_fetch = arm_fetch(p, instruction);
    if(res_fetch == -1){
        return -1;
    }
    int res_cond = arm_instruction_condition(p, instruction);
    if(res_cond == -1){
        return -1;
    }
    int res_decode = arm_instruction_type_decode(p, instruction);

    return 0;
}

int arm_step(arm_core p) {
    int result;

    result = arm_execute_instruction(p);
    if (result) {
        return arm_exception(p, result);
    }
    return result;
}
