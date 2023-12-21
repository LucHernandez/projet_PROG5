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

// Tous les codes des opérations arithmétiques se ressemblant, on utilisera cette fonction avec les infos
// récupérées des différentes opérations
int execute_operation(arm_core p, uint32_t ins, uint64_t res, uint8_t rd, uint8_t rn, uint32_t shifter_op){
	// On écrit le résultat dans le registre de destination
	arm_write_register(p, rd, (uint32_t)res);

	if (get_bit(ins, 20) == 1 && rd == 15){
		if (arm_current_mode_has_spsr(p)){
			arm_write_cpsr(p, arm_read_spsr(p));
		} else {
			// UNPREDICTABLE
			return -1;
		}
	}
	else if (get_bit(ins, 20) == 1){
		uint32_t n = get_bit(arm_read_register(p, rd), N)<<N;
		uint32_t z = ((res) == (0) ? (1<<Z) : (0<<Z));
		uint32_t c = ((arm_read_register(p, rd)) == (res) ? (0<<C) : (1<<C));
		uint32_t v = 0<<V;
		if (get_bit(shifter_op, 31)==get_bit(arm_read_register(p, rn), 31) && get_bit(shifter_op, 31)!=get_bit(res, 31)){
			v = 1<<V;
		}
		uint32_t cpsr_left = get_bits(arm_read_cpsr(p), 27, 0);
		arm_write_cpsr(p, (n+z+c+v+cpsr_left));
	}
	return 0;
}

void execute_sub(arm_core p, uint32_t ins){
	uint8_t rd = get_bits(ins, 15, 12);
	uint8_t rn = get_bits(ins, 19, 16);
	uint32_t shifter_op = get_shifter_operand(p, ins, NULL);
	uint64_t res = (uint64_t)arm_read_register(p, rn) - (uint64_t)shifter_op;

	execute_operation(p, ins, res, rd, rn, shifter_op);
}

void execute_rsb(arm_core p, uint32_t ins){
	uint8_t rd = get_bits(ins, 15, 12);
	uint8_t rn = get_bits(ins, 19, 16);
	uint32_t shifter_op = get_shifter_operand(p, ins, NULL);
	uint64_t res = (uint64_t)shifter_op - (uint64_t)arm_read_register(p, rn);

	execute_operation(p, ins, res, rd, rn, shifter_op);
}

void execute_sbc(arm_core p, uint32_t ins){
	uint8_t rd = get_bits(ins, 15, 12);
	uint8_t rn = get_bits(ins, 19, 16);
	uint32_t shifter_op = get_shifter_operand(p, ins, NULL);
	uint64_t res = (uint64_t)arm_read_register(p, rn) - (uint64_t)shifter_op - (uint64_t)(~get_bit(arm_read_cpsr(p), C));

	execute_operation(p, ins, res, rd, rn, shifter_op);
}

void execute_rsc(arm_core p, uint32_t ins){
	uint8_t rd = get_bits(ins, 15, 12);
	uint8_t rn = get_bits(ins, 19, 16);
	uint32_t shifter_op = get_shifter_operand(p, ins, NULL);
	uint64_t res = (uint64_t)shifter_op - (uint64_t)arm_read_register(p, rn) - (uint64_t)(~get_bit(arm_read_cpsr(p), C));

	execute_operation(p, ins, res, rd, rn, shifter_op);
}

void execute_add(arm_core p, uint32_t ins){
	uint8_t rd = get_bits(ins, 15, 12);
	uint8_t rn = get_bits(ins, 19, 16);
	uint32_t shifter_op = get_shifter_operand(p, ins, NULL);
	uint64_t res = (uint64_t)arm_read_register(p, rn) + (uint64_t)shifter_op;

	execute_operation(p, ins, res, rd, rn, shifter_op);
}

void execute_adc(arm_core p, uint32_t ins){
	uint8_t rd = get_bits(ins, 15, 12);
	uint8_t rn = get_bits(ins, 19, 16);
	uint32_t shifter_op = get_shifter_operand(p, ins, NULL);
	uint64_t res = (uint64_t)arm_read_register(p, rn) + (uint64_t)shifter_op + (uint64_t)get_bit(arm_read_cpsr(p), C);

	execute_operation(p, ins, res, rd, rn, shifter_op);
}

void execute_mov(arm_core p, uint32_t ins){
	uint8_t rd = get_bits(ins, 15, 12);
	uint8_t carry_out;
	uint32_t res = get_shifter_operand(p, ins, carry_out);

		execute_moving(p, ins, res, rd);
}

void execute_mvn(arm_core p, uint32_t ins){
	uint8_t rd = get_bits(ins, 15, 12);
	uint8_t carry_out;
	uint32_t res = ~(get_shifter_operand(p, ins, carry_out));

	execute_moving(p, ins, res, rd);
}

int execute_moving(arm_core p, uint32_t ins, uint64_t res, uint8_t rd){
	arm_write_register(p, rd, res);

	if (get_bit(ins, 20) == 1 && rd == 15){
		if (arm_current_mode_has_spsr(p)){
			arm_write_cpsr(p, arm_read_spsr(p));
		} else {
			// UNPREDICTABLE
			return -1;
		}
	}
	else if (get_bit(ins, 20) == 1){
		uint32_t n = get_bit(arm_read_register(p, rd), N) << N;
		uint32_t z = ((res) == (0) ? (1<<Z) : (0<<Z));
		uint32_t c = get_bit(res, C) << C;
		uint32_t cpsr_left = get_bits(arm_read_cpsr(p), 28, 0);
		arm_write_cpsr(p, (n+z+c+cpsr_left));
	}
	return 0;
}