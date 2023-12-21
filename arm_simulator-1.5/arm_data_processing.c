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

int AND(arm_core p, uint32_t ins){
    return UNDEFINED_INSTRUCTION;
}
int EOR(arm_core p, uint32_t ins){
    return UNDEFINED_INSTRUCTION;
}
int SUB(arm_core p, uint32_t ins){
    return UNDEFINED_INSTRUCTION;
}
int RSB(arm_core p, uint32_t ins){
    return UNDEFINED_INSTRUCTION;
}
int ADD(arm_core p, uint32_t ins){
    return UNDEFINED_INSTRUCTION;
}
int ADC(arm_core p, uint32_t ins){
    return UNDEFINED_INSTRUCTION;
}
int SBC(arm_core p, uint32_t ins){
    return UNDEFINED_INSTRUCTION;
}
int RSC(arm_core p, uint32_t ins){
    return UNDEFINED_INSTRUCTION;
}
int TST(arm_core p, uint32_t ins){
    return UNDEFINED_INSTRUCTION;
}
int TEQ(arm_core p, uint32_t ins){
    return UNDEFINED_INSTRUCTION;
}
int CMP(arm_core p, uint32_t ins){
    return UNDEFINED_INSTRUCTION;
}
int CMN(arm_core p, uint32_t ins){
    return UNDEFINED_INSTRUCTION;
}
int ORR(arm_core p, uint32_t ins){
    return UNDEFINED_INSTRUCTION;
}
int MOV(arm_core p, uint32_t ins){
    return UNDEFINED_INSTRUCTION;
}
int BIC(arm_core p, uint32_t ins){
    return UNDEFINED_INSTRUCTION;
}
int MVN(arm_core p, uint32_t ins){
    return UNDEFINED_INSTRUCTION;
}

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
			err_instru = AND(p, ins);
			return err_instru;
		case 0b0001:
			err_instru = EOR(p, ins);
			return err_instru;
		case 0b0010:
			err_instru = SUB(p, ins);
			return err_instru;
		case 0b0011:
			err_instru = RSB(p, ins);
			return err_instru;
		case 0b0100:
			err_instru = ADD(p, ins);
			return err_instru;
		case 0b0101:
			err_instru = ADC(p, ins);
			return err_instru;
		case 0b0110:
			err_instru = SBC(p, ins);
			return err_instru;
		case 0b0111:
			err_instru = RSC(p, ins);
			return err_instru;
		case 0b1000:
			if(bit_20 == 1){
				err_instru = TST(p, ins);
				return err_instru;
			}
			return UNDEFINED_INSTRUCTION;
		case 0b1001:
			if(bit_20 == 1){
				err_instru = TEQ(p, ins);
				return err_instru;
			}
			return UNDEFINED_INSTRUCTION;
		case 0b1010:
			if(bit_20 == 1){
				err_instru = CMP(p, ins);
				return err_instru;
			}
			return UNDEFINED_INSTRUCTION;
		case 0b1011:
			if(bit_20 == 1){
				err_instru = CMN(p, ins);
				return err_instru;
			}
			return UNDEFINED_INSTRUCTION;
		case 0b1100:
			err_instru = ORR(p, ins);
			return err_instru;
		case 0b1101:
			err_instru = MOV(p, ins);
			return err_instru;
		case 0b1110:
			err_instru = BIC(p, ins);
			return err_instru;
		case 0b1111:
			err_instru = MVN(p, ins);
			return err_instru;
		default:
			return UNDEFINED_INSTRUCTION;
		}
}
