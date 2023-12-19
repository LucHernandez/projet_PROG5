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
#include "arm_data_processing.h"
#include "arm_exception.h"
#include "arm_constants.h"
#include "arm_branch_other.h"
#include "util.h"
#include "debug.h"

uint32_t get_shifter_operand_LsL_imm(arm_core p, uint32_t instruction, uint8_t *carry_out);
uint32_t get_shifter_operand_LsL_reg(arm_core p, uint32_t instruction, uint8_t *carry_out);
uint32_t get_shifter_operand_LsR_imm(arm_core p, uint32_t instruction, uint8_t *carry_out);
uint32_t get_shifter_operand_LsR_reg(arm_core p, uint32_t instruction, uint8_t *carry_out);
uint32_t get_shifter_operand_AsR_imm(arm_core p, uint32_t instruction, uint8_t *carry_out);
uint32_t get_shifter_operand_AsR_reg(arm_core p, uint32_t instruction, uint8_t *carry_out);
uint32_t get_shifter_operand_RoR_imm(arm_core p, uint32_t instruction, uint8_t *carry_out);
uint32_t get_shifter_operand_RoR_reg(arm_core p, uint32_t instruction, uint8_t *carry_out);

/* Decoding functions for different classes of instructions */
int arm_data_processing_shift(arm_core p, uint32_t ins) {
    return UNDEFINED_INSTRUCTION;
}

int arm_data_processing_immediate_msr(arm_core p, uint32_t ins) {
    return UNDEFINED_INSTRUCTION;
}

uint32_t get_shifter_operand(arm_core p, uint32_t instruction, uint8_t *carry_out) {
	uint32_t shifter_operand = 0;

	// Differentes facons de traiter le shifter_operand page 442 de la doc
	// On commence par traiter les differents cas "particuliers"
    if (get_bit(instruction, 25)) { // Verifie le bit i pour savoir si on prend un immediate ou pas
        shifter_operand = ror(get_bits(instruction, 7, 0), get_bits(instruction, 11, 8));
		if (get_bits(instruction, 11, 8) == 0 && carry_out != NULL) {
			*carry_out = get_bit(arm_read_cpsr(p), C);
		}
    }
    else if (get_bits(instruction, 11, 4) == 0) { // Verif si on a juste un reg sans rotate
        shifter_operand = arm_read_register(p, get_bits(instruction, 3, 0));
		if (carry_out != NULL) {
			*carry_out = get_bit(arm_read_cpsr(p), C);
		}
    }
	else if (get_bits(instruction, 11, 4) == 0b110) { // Verif si on a rotate with right extend
		uint32_t shifter_operand = (get_bit(arm_read_cpsr(p), C) << 31) | (arm_read_register(p, get_bits(instruction, 3, 0)) >> 1);
		if (carry_out != NULL) {
			*carry_out = get_bit(arm_read_register(p, get_bits(instruction, 3, 0)), 0);
		}
	}

	// Sinon on a un joli tableau de correspondance :D

}

uint32_t get_shifter_operand_LsL_imm(arm_core p, uint32_t instruction, uint8_t *carry_out) {
	
}
uint32_t get_shifter_operand_LsL_reg(arm_core p, uint32_t instruction, uint8_t *carry_out) {
	
}
uint32_t get_shifter_operand_LsR_imm(arm_core p, uint32_t instruction, uint8_t *carry_out) {
	
}
uint32_t get_shifter_operand_LsR_reg(arm_core p, uint32_t instruction, uint8_t *carry_out) {
	
}
uint32_t get_shifter_operand_AsR_imm(arm_core p, uint32_t instruction, uint8_t *carry_out) {
	
}
uint32_t get_shifter_operand_AsR_reg(arm_core p, uint32_t instruction, uint8_t *carry_out) {
	
}
uint32_t get_shifter_operand_RoR_imm(arm_core p, uint32_t instruction, uint8_t *carry_out) {
	
}
uint32_t get_shifter_operand_RoR_reg(arm_core p, uint32_t instruction, uint8_t *carry_out) {
	
}