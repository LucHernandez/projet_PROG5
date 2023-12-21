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
#ifndef __ARM_DATA_PROCESSING_H__
#define __ARM_DATA_PROCESSING_H__
#include <stdint.h>
#include "arm_core.h"

int arm_data_processing_instructions(arm_core p, uint32_t ins);

int AND(arm_core p, uint32_t ins);
int EOR(arm_core p, uint32_t ins);
int SUB(arm_core p, uint32_t ins);
int RSB(arm_core p, uint32_t ins);
int ADD(arm_core p, uint32_t ins);
int ADC(arm_core p, uint32_t ins);
int SBC(arm_core p, uint32_t ins);
int RSC(arm_core p, uint32_t ins);
int TST(arm_core p, uint32_t ins);
int TEQ(arm_core p, uint32_t ins);
int CMP(arm_core p, uint32_t ins);
int CMN(arm_core p, uint32_t ins);
int ORR(arm_core p, uint32_t ins);
int MOV(arm_core p, uint32_t ins);
int BIC(arm_core p, uint32_t ins);
int MVN(arm_core p, uint32_t ins);

#endif
