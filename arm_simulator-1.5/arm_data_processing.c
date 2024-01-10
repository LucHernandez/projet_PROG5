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
int arm_data_processing_instructions(arm_core p, uint32_t ins) {
	int bits_27_25 = get_bits(ins, 27, 25);
	int bits_24_21 = get_bits(ins, 24, 21);
	int bit_20 = get_bit(ins, 20);

	if(bits_27_25 != 0b000 && bits_27_25 != 0b001){
		return UNDEFINED_INSTRUCTION;
	}

	int err_instru;
	switch(bits_24_21){
		case 0b0000:
			err_instru = arm_data_processing_AND(p, ins);
			return err_instru;
		case 0b0001:
			err_instru = arm_data_processing_EOR(p, ins);
			return err_instru;
		case 0b0010:
			err_instru = arm_data_processing_SUB(p, ins);
			return err_instru;
		case 0b0011:
			err_instru = arm_data_processing_RSB(p, ins);
			return err_instru;
		case 0b0100:
			err_instru = arm_data_processing_ADD(p, ins);
			return err_instru;
		case 0b0101:
			err_instru = arm_data_processing_ADC(p, ins);
			return err_instru;
		case 0b0110:
			err_instru = arm_data_processing_SBC(p, ins);
			return err_instru;
		case 0b0111:
			err_instru = arm_data_processing_RSC(p, ins);
			return err_instru;
		case 0b1000:
			if(bit_20 == 1){
				err_instru = arm_data_processing_TST(p, ins);
				return err_instru;
			}
			return UNDEFINED_INSTRUCTION;
		case 0b1001:
			if(bit_20 == 1){
				err_instru = arm_data_processing_TEQ(p, ins);
				return err_instru;
			}
			return UNDEFINED_INSTRUCTION;
		case 0b1010:
			if(bit_20 == 1){
				err_instru = arm_data_processing_CMP(p, ins);
				return err_instru;
			}
			return UNDEFINED_INSTRUCTION;
		case 0b1011:
			if(bit_20 == 1){
				err_instru = arm_data_processing_CMN(p, ins);
				return err_instru;
			}
			return UNDEFINED_INSTRUCTION;
		case 0b1100:
			err_instru = arm_data_processing_ORR(p, ins);
			return err_instru;
		case 0b1101:
			err_instru = arm_data_processing_MOV(p, ins);
			return err_instru;
		case 0b1110:
			err_instru = arm_data_processing_BIC(p, ins);
			return err_instru;
		case 0b1111:
			err_instru = arm_data_processing_MVN(p, ins);
			return err_instru;
		default:
			return UNDEFINED_INSTRUCTION;
		}
}

// Fonction de calcul des flags des operations arithmetiques et logiques et execution de celles-ci
int execute_operation(arm_core p, uint32_t ins, long res, uint8_t rd, uint8_t rn, uint32_t shifter_op, uint8_t carry_out, int cas){
	
	// equivalent au resultat 64 bits mais sur 32 pour pouvoir faire les comparaisons lors des calculs des flags
	uint32_t alu_out = (uint32_t)res;
	uint32_t n = get_bit(res, N) << N;
	uint32_t z = (alu_out == 0 ? 1 : 0) << Z;
	uint32_t c = ((long)alu_out == res ? 0 : 1) << C;
	uint32_t v;

	switch (cas){
		// cas de l'addition
		case 1:
			v = ((get_bit(shifter_op, 31) == get_bit(arm_read_register(p, rn), 31) && get_bit(shifter_op, 31) != get_bit(res, 31)) ? 1 : 0) << V;
			break;
		// cas de la soustraction
		case 2:
	        c = ((long)alu_out == res ? 1 : 0) << C;
			v = ((get_bit(shifter_op, 31) != get_bit(arm_read_register(p, rn), 31) && get_bit(shifter_op, 31) == get_bit(res, 31)) ? 1 : 0) << V;
			break;
		// cas d'operations logiques
		case 3:
			c = carry_out << C;
			v = get_bit(arm_read_cpsr(p), V) << V;
			break;
	}
	
	uint32_t cpsr = get_bits(arm_read_cpsr(p), 27, 0);
	
	// Pour les cas des opérations de comparaison qui ne necessitent pas d'ecrire dans un registre
	if (rd == 255){				
		arm_write_cpsr(p, cpsr | n | z | c | v);
	} 
	// Pour les autres cas avec un registre de destination
	else {
		// On ecrit dans le-dit registre
		arm_write_register(p, rd, (uint32_t)res);
		
		if (get_bit(ins, 20) == 1 && rd == 15){
			if (arm_current_mode_has_spsr(p)){
				arm_write_cpsr(p, arm_read_spsr(p));
				return 0;
			} else {
				// UNPREDICTABLE
				return -1;
			}
		}
		else if (get_bit(ins, 20) == 1){
			arm_write_cpsr(p, (cpsr | n | z | c | v));
		}
	}
	return 0;
}


/***************************************************************************************
***************************** SUBBING OPERATIONS ***************************************
****************************************************************************************/

int arm_data_processing_SUB(arm_core p, uint32_t ins){
	uint8_t rd = get_bits(ins, 15, 12);
	uint8_t rn = get_bits(ins, 19, 16);
	uint32_t shifter_op = get_shifter_operand(p, ins, NULL);

	long res = (long)arm_read_register(p, rn) - (long)shifter_op;

	return execute_operation(p, ins, res, rd, rn, shifter_op, (uint8_t)255, 2);
}

int arm_data_processing_RSB(arm_core p, uint32_t ins){
	uint8_t rd = get_bits(ins, 15, 12);
	uint8_t rn = get_bits(ins, 19, 16);
	uint32_t shifter_op = get_shifter_operand(p, ins, NULL);
	long res = (long)shifter_op - (long)arm_read_register(p, rn);

	return execute_operation(p, ins, res, rd, rn, shifter_op, (uint8_t)255, 2);
}

int arm_data_processing_SBC(arm_core p, uint32_t ins){
	uint8_t rd = get_bits(ins, 15, 12);
	uint8_t rn = get_bits(ins, 19, 16);
	uint32_t shifter_op = get_shifter_operand(p, ins, NULL);
	long res = (long)arm_read_register(p, rn) - (long)shifter_op - (long)(get_bit(arm_read_cpsr(p), C) ? 0 : 1);

	return execute_operation(p, ins, res, rd, rn, shifter_op, (uint8_t)255, 2);
}

int arm_data_processing_RSC(arm_core p, uint32_t ins){
	uint8_t rd = get_bits(ins, 15, 12);
	uint8_t rn = get_bits(ins, 19, 16);
	uint32_t shifter_op = get_shifter_operand(p, ins, NULL);
	long res = (long)shifter_op - (long)arm_read_register(p, rn) - (long)(get_bit(arm_read_cpsr(p), C) ? 0 : 1);

	return execute_operation(p, ins, res, rd, rn, shifter_op, (uint8_t)255, 2);
}

int arm_data_processing_CMP(arm_core p, uint32_t ins){
	uint8_t rn = get_bits(ins, 19, 16);
	uint32_t shifter_op = get_shifter_operand(p, ins, NULL);
	long res = (long)arm_read_register(p, rn) - (long)shifter_op;

	return execute_operation(p, ins, res, (uint8_t)255, rn, shifter_op, (uint8_t)255, 2);
}

/***************************************************************************************
****************************** ADDING OPERATIONS ***************************************
****************************************************************************************/

int arm_data_processing_ADD(arm_core p, uint32_t ins){
	uint8_t rd = get_bits(ins, 15, 12);
	uint8_t rn = get_bits(ins, 19, 16);
	uint32_t shifter_op = get_shifter_operand(p, ins, NULL);
	long res = (long)arm_read_register(p, rn) + (long)shifter_op;

	return execute_operation(p, ins, res, rd, rn, shifter_op, (uint8_t)255, 1);
}

int arm_data_processing_ADC(arm_core p, uint32_t ins){
	uint8_t rd = get_bits(ins, 15, 12);
	uint8_t rn = get_bits(ins, 19, 16);
	uint32_t shifter_op = get_shifter_operand(p, ins, NULL);
	long res = (long)arm_read_register(p, rn) + (long)shifter_op + (long)get_bit(arm_read_cpsr(p), C);

	return execute_operation(p, ins, res, rd, rn, shifter_op, (uint8_t)255, 1);
}

int arm_data_processing_CMN(arm_core p, uint32_t ins){
	uint8_t rn = get_bits(ins, 19, 16);
	uint32_t shifter_op = get_shifter_operand(p, ins, NULL);
	long res = (long)arm_read_register(p, rn) + (long)shifter_op;

	return execute_operation(p, ins, res, (uint8_t)255, rn, shifter_op, (uint8_t)255, 1);
}

/***************************************************************************************
************************ SECTION OPERATIONS LOGIQUES ***********************************
****************************************************************************************/

int arm_data_processing_AND(arm_core p, uint32_t ins){
	uint8_t rd = get_bits(ins, 15, 12);
	uint8_t rn = get_bits(ins, 19, 16);
	uint8_t carry_out = 0;
	uint32_t shifter_op = get_shifter_operand(p, ins, &carry_out);
	long res = arm_read_register(p, rn) & shifter_op;
	arm_write_register(p, rd, res);
	return execute_operation(p, ins, res, rd, rn, shifter_op, carry_out, 3);
}

int arm_data_processing_EOR(arm_core p, uint32_t ins){
	uint8_t rd = get_bits(ins, 15, 12);
	uint8_t rn = get_bits(ins, 19, 16);
	uint8_t carry_out = 0;
	uint32_t shifter_op = get_shifter_operand(p, ins, &carry_out);
	long res = arm_read_register(p, rn) ^ shifter_op;
	
	return execute_operation(p, ins, res, rd, rn, shifter_op, carry_out, 3);
}

int arm_data_processing_TST(arm_core p, uint32_t ins){
	uint8_t rn = get_bits(ins, 19, 16);
	uint8_t carry_out = 0;
	uint32_t shifter_op = get_shifter_operand(p, ins, &carry_out);
	long res = arm_read_register(p, rn) & shifter_op;
	
	return execute_operation(p, ins, res, (uint8_t)255, rn, shifter_op, carry_out, 3);
}

int arm_data_processing_TEQ(arm_core p, uint32_t ins){
	uint8_t rn = get_bits(ins, 19, 16);
	uint8_t carry_out = 0;
	uint32_t shifter_op = get_shifter_operand(p, ins, &carry_out);
	long res = arm_read_register(p, rn) ^ shifter_op;
	
	return execute_operation(p, ins, res, (uint8_t)255, rn, shifter_op, carry_out, 3);
}

int arm_data_processing_ORR(arm_core p, uint32_t ins){
	uint8_t rd = get_bits(ins, 15, 12);
	uint8_t rn = get_bits(ins, 19, 16);
	uint8_t carry_out = 0;
	uint32_t shifter_op = get_shifter_operand(p, ins, &carry_out);
	long res = arm_read_register(p, rn) | shifter_op;
	
	return execute_operation(p, ins, res, rd, rn, shifter_op, carry_out, 3);
}

int arm_data_processing_BIC(arm_core p, uint32_t ins){
	uint8_t rd = get_bits(ins, 15, 12);
	uint8_t rn = get_bits(ins, 19, 16);
	uint8_t carry_out = 0;
	uint32_t shifter_op = get_shifter_operand(p, ins, &carry_out);
	long res = arm_read_register(p, rn) & ~shifter_op;
	
	return execute_operation(p, ins, res, rd, rn, shifter_op, carry_out, 3);
}

int arm_data_processing_MOV(arm_core p, uint32_t ins){
	uint8_t rd = get_bits(ins, 15, 12);
	uint8_t carry_out = 0;
	uint32_t shifter_op = get_shifter_operand(p, ins, &carry_out);
	arm_write_register(p, rd, shifter_op);

	return execute_operation(p, ins, (long)arm_read_register(p, rd), rd, (uint8_t)255, shifter_op, carry_out, 3);
}

int arm_data_processing_MVN(arm_core p, uint32_t ins){
	uint8_t rd = get_bits(ins, 15, 12);
	uint8_t carry_out = 0;
	uint32_t shifter_op = get_shifter_operand(p, ins, &carry_out);
	arm_write_register(p, rd, ~shifter_op);

	return execute_operation(p, ins, (long)arm_read_register(p, rd), rd, (uint8_t)255, shifter_op, carry_out, 3);
}

/***************************************************************************************
******************** SECTION RECUPERATION SHIFTER OPERAND ******************************
****************************************************************************************/

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

