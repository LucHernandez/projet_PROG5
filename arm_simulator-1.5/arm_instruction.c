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
                return 0;
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

int arm_instruction_type_decode(arm_core p, uint32_t instruction, int code){ //p.110s
    int bits_25_27 = get_bits(instruction, 27, 25);
    int bits_24_20 = get_bits(instruction, 24, 20);
    int bits_7_4 = get_bits(instruction, 7, 4);
    int bit_24 = get_bit(instruction, 24);
    int bit_23 = get_bit(instruction, 23);
    int bit_21 = get_bit(instruction, 21);
    int bit_20 = get_bit(instruction, 20);
    int bit_7 = get_bit(instruction, 7);
    int bit_4 = get_bit(instruction, 4);

    int err_instru;
    if (code == 1 && (bits_25_27 != 0b110 || bits_25_27 != 0b111)) //test si condition instruction est 1111 si oui alors on est dans unconditional instructions 
    {
        return UNDEFINED_INSTRUCTION;
    }
    switch(bits_25_27)
    {
    case 0b000:
        if(bit_24 == 1 && bit_23 == 0 && bit_20 == 0 && bit_4 == 0 ){ //miscellaneous instruction
            err_instru = arm_miscellaneous(p, instruction);
            return err_instru;
        }
        if(bit_4 == 0){ //data processing immediate shift
            err_instru = arm_data_processing_instructions(p, instruction);
            return err_instru;
        }
        if(bit_24 == 1 && bit_23 == 0 && bit_20 == 0 && bit_7 == 0 && bit_4 == 1 ){ //miscellaneous instruction
            err_instru = arm_miscellaneous(p, instruction);
            return err_instru;
        }
        if(bit_7 == 0 && bit_4 == 1){ //data processing register shift
            err_instru = arm_data_processing_instructions(p, instruction);
            return err_instru;
        }
        if(bit_7 == 1 && bit_4 == 1){ //multiplies or extra load/store
            err_instru = arm_load_store(p, instruction); //cas du ldrh et du strh 
            //si ajout des fonctions de multiplication utiliser une deuxième variable pour le err_instru des multiplications
            return err_instru;
        }
        return UNDEFINED_INSTRUCTION;
    case 0b001:
        if(bit_24 == 1 && bit_23 == 0 && bit_21 == 0 && bit_20 == 0){ //undefined instruction
            return UNDEFINED_INSTRUCTION;
        }
        if(bit_24 == 1 && bit_23 == 0 && bit_21 == 1 && bit_20 == 0){ //move immediate to status register c'est dans les miscellaneous
            err_instru = arm_miscellaneous(p, instruction);
            return err_instru;
        }
        err_instru = arm_data_processing_instructions(p, instruction); //data processing immediate
        return err_instru;
    case 0b010: //load/store immediate offset
        err_instru = arm_load_store(p, instruction);
        return err_instru;
    case 0b011:
        if(bits_24_20 == 0b11111 && bits_7_4 == 0b1111){ //architecturally undefined
            return UNDEFINED_INSTRUCTION;
        }
        if(bit_4 == 1){ //media instructions
            return UNDEFINED_INSTRUCTION;
        }
        if(bit_4 == 0){ //load/store register offset
            err_instru = arm_load_store(p, instruction);
            return err_instru;
        }
        return UNDEFINED_INSTRUCTION;
    case 0b100: //load/store multiple
        err_instru = arm_load_store_multiple(p, instruction);
        return err_instru;
    case 0b101: //branch and branch with link
        err_instru = arm_branch(p, instruction);
        return err_instru;
    case 0b110: // coprocessor ldr/str and double register transfer
        err_instru = arm_coprocessor_load_store(p, instruction);
        return err_instru;
    case 0b111:
        if(bit_24 == 0 && bit_4 == 0){ //coprocessor data processing
            return UNDEFINED_INSTRUCTION;
        }
        if(bit_24 == 0 && bit_4 == 1){ //coprocessor register transfers
            err_instru = arm_coprocessor_load_store(p, instruction);
            return err_instru;
        }
        if(bit_24 == 1 && code != 1){ //software interrupt test de code si code à 1 alors condition == 1111 et on est alors en unconditionnal instructions
            err_instru = arm_coprocessor_others_swi(p, instruction);
            return err_instru;
        }
        return UNDEFINED_INSTRUCTION;
    default:
        return UNDEFINED_INSTRUCTION;
    }
}

static int arm_execute_instruction(arm_core p) {
    uint32_t instruction = 0;
    int res_fetch = arm_fetch(p, &instruction);
    if(res_fetch == -1){
        return -1;
    }
    int res_cond = arm_instruction_condition(p, instruction);
    if(res_cond == -1){
        return -1;
    }
    int res_decode = arm_instruction_type_decode(p, instruction, res_cond);
    return res_decode;
}

int arm_step(arm_core p) {
    int result;

    result = arm_execute_instruction(p);
    if (result) {
        return arm_exception(p, result);
    }
    return result;
}
