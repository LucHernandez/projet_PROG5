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
#ifndef __ARM_LOAD_STORE_H__
#define __ARM_LOAD_STORE_H__
#include <stdint.h>
#include "arm_core.h"

int arm_load_store(arm_core p, uint32_t ins);
int arm_load_store_multiple(arm_core p, uint32_t ins);
int arm_coprocessor_load_store(arm_core p, uint32_t ins);

uint32_t Shift_case(arm_core p,uint32_t ins,uint32_t RmVal);
uint8_t calcul_adresse(arm_core p,uint32_t ins,int flag_index,int flag_Equivalent,uint32_t* addr,uint32_t value);
int Recup_addresse_WORD_BYTE_HALF(arm_core p,uint32_t ins,uint32_t* addr,int flag_Half);
//uint8_t addr_mode_H(arm_core p,uint32_t ins,uint32_t *addr);
uint8_t addr_mode_MULTIPLE(arm_core p,uint32_t ins,uint32_t *start_address,uint32_t *end_address);
int Number_Of_Set_Bits_In(uint32_t ins);
int If_Kevin(arm_core p,uint32_t ins,uint32_t* RmVal,uint8_t* RmNum,uint32_t* index);


int arm_load_store_STR(arm_core p,uint32_t ins);
int arm_load_store_STRB(arm_core p,uint32_t ins);
int arm_load_store_STRH(arm_core p,uint32_t ins);
int arm_load_store_STM(arm_core p,uint32_t ins);
int arm_load_store_LDR(arm_core p,uint32_t ins);
int arm_load_store_LDRB(arm_core p,uint32_t ins);
int arm_load_store_LDRH(arm_core p,uint32_t ins);
int arm_load_store_LDM(arm_core p,uint32_t ins);




#endif