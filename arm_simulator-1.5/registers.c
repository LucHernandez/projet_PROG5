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
#include "registers.h"
#include "arm_constants.h"
#include <stdlib.h>

struct registers_data {
    /* � compl�ter... */
};

registers registers_create() {
    registers r = NULL;
    /* � compl�ter... */
    return r;
}

void registers_destroy(registers r) {
    /* � compl�ter... */
}

uint8_t registers_get_mode(registers r) {
    /* � compl�ter... */
    return SVC;
}

static int registers_mode_has_spsr(registers r, uint8_t mode) {
    /* � compl�ter... */
    return 1;
}

int registers_current_mode_has_spsr(registers r) {
    return registers_mode_has_spsr(r, registers_get_mode(r));
}

int registers_in_a_privileged_mode(registers r) {
    /* � compl�ter... */
    return 0;
}

int get_mode_ligne (uint8_t mode){
    //ont recupere l'index de la ligne du tableau qui corespond au mode
    switch (mode) 
    {
        case USR :
            return 0;
            break;
        case SYS :
            return 1;
            break;
        case SVC :
            return 2;
            break;
        case ABT :
            return 3;
            break;
        case UND :
            return 4;
            break;
        case IRQ :
            return 5;
            break;
        case FIQ :
            return 6;
            break;
        default :
            exit(2);
            break;
    };
}

uint32_t registers_read(registers r, uint8_t reg, uint8_t mode) {
    uint32_t value = 0;
    int ligne=get_mode_ligne(mode);  //ligne corespondant au mode

    //ont recupere la valeur du registre
    value=r->correspondance_modes[ligne][reg];

    return value;
}

uint32_t registers_read_cpsr(registers r) {
    uint32_t value = 0;

    //ont, renvois le registre CPSR qui est le meme pour chaque mode
    value=r->correspondance_modes[0][16];

    return value;
}

uint32_t registers_read_spsr(registers r, uint8_t mode) {
    uint32_t value = 0;

    // test si le mode a accee au registre SPSR
    if (registers_mode_has_spsr(r,mode)){
        value=correspondance_modes[get_mode_ligne(mode)][17];
    }
    else{
        //exit si le mode ne le permer pas
        exit(2);
    }

    return value;
}

void registers_write(registers r, uint8_t reg, uint8_t mode, uint32_t value) {
    //ont recupere la valeur du registre
    int ligne=get_mode_ligne(mode); //ligne corespondant au mode

    //ont change la valeur du registre
    *(r->correspondance_modes[ligne][reg])=value;
}

void registers_write_cpsr(registers r, uint32_t value) {
    *(r->correspondance_modes[0][16])=value;
}

void registers_write_spsr(registers r, uint8_t mode, uint32_t value) {
    // test si le mode a accee au registre SPSR
    if (registers_mode_has_spsr(r,mode)){
        *(correspondance_modes[get_mode_ligne(mode)][17])=value;
    }
    else{
        //exit si le mode ne le permer pas
        exit(2);
    }
}
