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
#ifndef __REGISTERS_H__
#define __REGISTERS_H__
#include <stdint.h>
#include <stdio.h>

typedef struct registers_data *registers;

registers registers_create();
void registers_destroy(registers r);

/* Schema pour trouver où le mode se situe dans CPSR
31 30 29 28 27 26 25 24 23 .. 20 19 .. 16 15 .. 10 9 8 7 6 5 4 .. 0
N  Z  S  V  Q  Res   J  RESERVED GE       RESERVED E A I F T MODE
*/
uint8_t registers_get_mode(registers r);
int registers_current_mode_has_spsr(registers r);
int registers_in_a_privileged_mode(registers r);

uint32_t registers_read(registers r, uint8_t reg, uint8_t mode);
uint32_t registers_read_cpsr(registers r);
uint32_t registers_read_spsr(registers r, uint8_t mode);
void registers_write(registers r, uint8_t reg, uint8_t mode, uint32_t value);
void registers_write_cpsr(registers r, uint32_t value);
void registers_write_spsr(registers r, uint8_t mode, uint32_t value);

int get_mode_ligne (uint8_t mode);

#endif
