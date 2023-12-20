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


int execute_AND(arm_core p, uint32_t instruction) {
	uint8_t rd = get_bits(instruction, 15, 12);
	uint8_t rn = get_bits(instruction, 19, 16);
	uint8_t update_cpsr = get_bit(instruction, 20);
	uint8_t carry_out = 0;
	uint32_t value = arm_read_register(p, rn) & get_shifter_operand(p, instruction, &carry_out);
	arm_write_register(p, rd, value);
	if (update_cpsr) {
		if (rd == 15 && arm_current_mode_has_spsr(p)) {
			arm_write_cpsr(p, arm_read_spsr(p));
		} else {
			if (!arm_current_mode_has_spsr(p)) return -1;
			uint32_t flags = ((get_bit(arm_read_register(p, rd), 31)) << 3) | ((arm_read_register(p, rd) == 0 ? 1 : 0) << 2) | (carry_out << 1);
			uint32_t cpsr = arm_read_cpsr(p) & ~(((uint32_t) 0b111) << 29);
			arm_write_cpsr(p, cpsr | flags << 28);
		}
	}
	return 0;
}

int execute_EOR(arm_core p, uint32_t instruction) {
	uint8_t rd = get_bits(instruction, 15, 12);
	uint8_t rn = get_bits(instruction, 19, 16);
	uint8_t update_cpsr = get_bit(instruction, 20);
	uint8_t carry_out = 0;
	uint32_t value = arm_read_register(p, rn) ^ get_shifter_operand(p, instruction, &carry_out);
	arm_write_register(p, rd, value);
	if (update_cpsr) {
		if (rd == 15 && arm_current_mode_has_spsr(p)) {
			arm_write_cpsr(p, arm_read_spsr(p));
		} else {
			if (!arm_current_mode_has_spsr(p)) return -1;
			uint32_t flags = ((get_bit(arm_read_register(p, rd), 31)) << 3) | ((arm_read_register(p, rd) == 0 ? 1 : 0) << 2) | (carry_out << 1);
			uint32_t cpsr = arm_read_cpsr(p) & ~(((uint32_t) 0b111) << 29);
			arm_write_cpsr(p, cpsr | flags << 28);
		}
	}
	return 0;
}

int execute_TST(arm_core p, uint32_t instruction) {
	uint8_t rn = get_bits(instruction, 19, 16);
	uint8_t carry_out = 0;
	uint32_t alu_out = arm_read_register(p, rn) & get_shifter_operand(p, instruction, &carry_out);
	uint32_t flags = ((get_bit(alu_out, 31)) << 3) | ((alu_out == 0 ? 1 : 0) << 2) | (carry_out << 1);
	uint32_t cpsr = arm_read_cpsr(p) & ~(((uint32_t) 0b111) << 29);
	arm_write_cpsr(p, cpsr | flags << 28);
	return 0;
}

int execute_TEQ(arm_core p, uint32_t instruction) {
	uint8_t rn = get_bits(instruction, 19, 16);
	uint8_t carry_out = 0;
	uint32_t alu_out = arm_read_register(p, rn) ^ get_shifter_operand(p, instruction, &carry_out);
	uint32_t flags = ((get_bit(alu_out, 31)) << 3) | ((alu_out == 0 ? 1 : 0) << 2) | (carry_out << 1);
	uint32_t cpsr = arm_read_cpsr(p) & ~(((uint32_t) 0b111) << 29);
	arm_write_cpsr(p, cpsr | flags << 28);
	return 0;
}

int execute_CMP(arm_core p, uint32_t instruction) {
	uint8_t rn = get_bits(instruction, 19, 16);
	uint8_t carry_out = 0;
	uint32_t v1 = arm_read_register(p, rn);
	uint32_t v2 = get_shifter_operand(p, instruction, &carry_out);
	uint32_t alu_out = v1 - v2;
	// voir borrow et overflow sur https://www.wikiwand.com/en/Integer_overflow#Flags
	uint8_t borrow = v2 > v1; // Borrow c'est le carry des soustractions
	uint8_t overflow = get_bit(v1, 31) != get_bit(v2, 31) && get_bit(alu_out, 31) == get_bit(v2, 31); // MsB != Carry (= borrow)
	uint32_t flags = ((get_bit(alu_out, 31)) << 3) | ((alu_out == 0 ? 1 : 0) << 2) | ((get_bit(!borrow, 0)) << 1) | (get_bit(overflow, 0));
	uint32_t cpsr = arm_read_cpsr(p) & ~(((uint32_t) 0b111) << 29);
	arm_write_cpsr(p, cpsr | flags << 28);
	return 0;
}

// int execute_CMN(arm_core p, uint32_t instruction) {
// 	uint8_t rn = get_bits(instruction, 19, 16);
// 	uint8_t carry_out = 0;
// 	uint32_t v1 = arm_read_register(p, rn);
// 	uint32_t v2 = get_shifter_operand(p, instruction, &carry_out);
// 	uint32_t alu_out = v1 + v2;
// 	uint8_t carry = 
// 	uint8_t overflow = (get_bit(v1, 31) == get_bit(v2, 31) && get_bit(alu_out, 31) != get_bit(v2, 31));
// 	return 0;
// }

//
uint32_t get_shifter_operand(arm_core p, uint32_t instruction, uint8_t *carry_out) {
	uint32_t shifter_operand = 0;

	// Differentes facons de traiter le shifter_operand page 442 de la doc
	// On commence par traiter les differents cas "particuliers"
    if (get_bit(instruction, 25)) { // Verifie le bit i pour savoir si on prend un immediate ou pas
        shifter_operand = ror(get_bits(instruction, 7, 0), get_bits(instruction, 11, 8));
		if (get_bits(instruction, 11, 8) == 0 && carry_out != NULL) {
			*carry_out = get_bit(arm_read_cpsr(p), C);
		}
		else if (carry_out != NULL) {
			*carry_out = get_bit(shifter_operand, 31);
		}
    }
    else if (get_bits(instruction, 11, 4) == 0) { // Verif si on a juste un reg sans rotate
        shifter_operand = arm_read_register(p, get_bits(instruction, 3, 0));
		if (carry_out != NULL) {
			*carry_out = get_bit(arm_read_cpsr(p), C);
		}
    }
	else if (get_bits(instruction, 11, 4) == 0b110) { // Verif si on a rotate with right extend
		shifter_operand = (get_bit(arm_read_cpsr(p), C) << 31) | (arm_read_register(p, get_bits(instruction, 3, 0)) >> 1);
		if (carry_out != NULL) {
			*carry_out = get_bit(arm_read_register(p, get_bits(instruction, 3, 0)), 0);
		}
	}
	else { // Sinon on a un joli tableau de correspondance :D
		uint32_t (*adressing_modes[8])(arm_core, uint32_t, uint8_t*) = {
			get_shifter_operand_LsL_imm,
			get_shifter_operand_LsL_reg,
			get_shifter_operand_LsR_imm,
			get_shifter_operand_LsR_reg,
			get_shifter_operand_AsR_imm, 
			get_shifter_operand_AsR_reg,
			get_shifter_operand_RoR_imm,
			get_shifter_operand_RoR_reg
		};
		shifter_operand = adressing_modes[get_bits(instruction, 6, 4)](p, instruction, carry_out);
	}

	return shifter_operand;
}

uint32_t get_shifter_operand_LsL_imm(arm_core p, uint32_t instruction, uint8_t *carry_out) {
	uint32_t rm = arm_read_register(p, get_bits(instruction, 3, 0));
	uint32_t shift_imm = get_bits(instruction, 11, 7);
	if (shift_imm == 0) {
		if (carry_out != NULL) *carry_out = get_bit(arm_read_cpsr(p), C);
		return rm;
	}
	if (carry_out != NULL) *carry_out = get_bit(rm, 32 - shift_imm);
	return rm << shift_imm;
}

uint32_t get_shifter_operand_LsL_reg(arm_core p, uint32_t instruction, uint8_t *carry_out) {
	uint32_t rs07 = get_bits(arm_read_register(p, get_bits(instruction, 11, 8)), 7, 0);
	uint32_t rm = arm_read_register(p, get_bits(instruction, 3, 0));
	if (rs07 == 0) {
		if (carry_out != NULL) *carry_out = get_bit(arm_read_cpsr(p), C);
		return rm;
	}
	else if (rs07 < 32) {
		if (carry_out != NULL) *carry_out = get_bit(rm, 32 - rs07);
		return rm << rs07;
	}
	else if (rs07 == 32) {
		if (carry_out != NULL) *carry_out = get_bit(rm, 0);
		return 0;
	}
	else {
		if (carry_out != NULL) *carry_out = 0;
		return 0;
	}
}

uint32_t get_shifter_operand_LsR_imm(arm_core p, uint32_t instruction, uint8_t *carry_out) {
	uint32_t shift_imm = get_bits(instruction, 11, 7);
	uint32_t rm = arm_read_register(p, get_bits(instruction, 3, 0));
	if (shift_imm == 0) {
		if (carry_out != NULL) *carry_out = get_bit(rm, 31);
		return 0;
	} else {
		if (carry_out != NULL) *carry_out = get_bit(rm, shift_imm - 1);
		return rm >> shift_imm;
	}
}

uint32_t get_shifter_operand_LsR_reg(arm_core p, uint32_t instruction, uint8_t *carry_out) {
	uint32_t Rs07 = get_bits(arm_read_register(p, get_bits(instruction, 11, 8)), 7, 0);
	uint32_t rm = arm_read_register(p, get_bits(instruction, 3, 0));
	if (Rs07 == 0) {
		if (carry_out != NULL) *carry_out = get_bit(arm_read_cpsr(p), C);
		return rm;
	}
	else if (Rs07 < 32) {
		if (carry_out != NULL) *carry_out = get_bit(rm, Rs07 - 1);
		return rm >> Rs07;
	}
	else if (Rs07 == 32) {
		if (carry_out != NULL) *carry_out = get_bit(rm, 31);
		return 0;
	}
	else {
		if (carry_out != NULL) *carry_out = 0;
		return 0;
	}
}

uint32_t get_shifter_operand_AsR_imm(arm_core p, uint32_t instruction, uint8_t *carry_out) {
	uint32_t shift_imm = get_bits(instruction, 11, 7);
	uint32_t rm = arm_read_register(p, get_bits(instruction, 3, 0));
	if (shift_imm != 0) {
		if (carry_out != NULL) *carry_out = get_bit(rm, shift_imm - 1);
		return asr(rm, shift_imm);
	}
	if (carry_out != NULL) *carry_out = get_bit(rm, 31);
	if (get_bit(rm, 31) == 0) {
		return 0;
	}
	return 0xFFFFFFFF;
}

uint32_t get_shifter_operand_AsR_reg(arm_core p, uint32_t instruction, uint8_t *carry_out) {
	uint32_t Rs07 = get_bits(arm_read_register(p, get_bits(instruction, 11, 8)), 7, 0);
	uint32_t rm = arm_read_register(p, get_bits(instruction, 3, 0));
	if (Rs07 == 0) {
		if (carry_out != NULL) *carry_out = get_bit(arm_read_cpsr(p), C);
		return rm;
	}
	else if (Rs07 < 32) {
		if (carry_out != NULL) *carry_out = get_bit(rm, Rs07 - 1);
		return asr(rm, Rs07);
	}
	if (get_bit(rm, 31) == 0) {
		if (carry_out != NULL) *carry_out = 0;
		return 0; // = rm[31]
	}
	if (carry_out != NULL) *carry_out = 1;
	return 0xFFFFFFFF; // = rm[31]
}

uint32_t get_shifter_operand_RoR_imm(arm_core p, uint32_t instruction, uint8_t *carry_out) {
	uint32_t shift_imm = get_bits(instruction, 11, 7);
	uint32_t rm = arm_read_register(p, get_bits(instruction, 3, 0));
	if (shift_imm == 0) {
		if (carry_out != NULL) *carry_out = get_bit(rm, 0);
		return (get_bit(arm_read_cpsr(p), C) << 31) | (rm >> 1);
	}
	if (carry_out != NULL) *carry_out = get_bit(rm, shift_imm - 1);
	return ror(rm, shift_imm);
}

uint32_t get_shifter_operand_RoR_reg(arm_core p, uint32_t instruction, uint8_t *carry_out) {
	uint32_t rs40 = get_bits(arm_read_register(p, get_bits(instruction, 11, 8)), 4, 0);
	uint32_t rm = arm_read_register(p, get_bits(instruction, 3, 0));
	if (get_bits(arm_read_register(p, get_bits(instruction, 11, 8)), 7, 0) == 0) {
		if (carry_out != NULL) *carry_out = get_bit(arm_read_cpsr(p), C);
		return rm;
	}
	if (rs40 == 0) {
		if (carry_out != NULL) *carry_out = get_bit(rm, 31);
		return rm;
	}
	if (carry_out != NULL) *carry_out = get_bit(rm, rs40 - 1);
	return ror(rm, rs40);
}