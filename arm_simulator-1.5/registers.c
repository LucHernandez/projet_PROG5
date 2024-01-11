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

#define SPSR 17

struct registers_data {
    // Voir page 43 Doc ARM pour correspondance des mods
    uint32_t *correspondance_modes[7][18];
    uint32_t tableau_registres[37];
};

typedef enum {R0, R1, R2, R3, R4, R5, R6, R7, R8, R9, R10, R11, R12, R13, R14, PC, CPSR, R13_SCV, R14_SCV, R13_ABT, R14_ABT, R13_UND, R14_UND, R13_IRQ, R14_IRQ, R8_FIQ, R9_FIQ, R10_FIQ, R11_FIQ, R12_FIQ, R13_FIQ, R14_FIQ, SPSR_SVC, SPSR_ABT, SPSR_UND, SPSR_IRQ, SPSR_FIQ} lien_registres_pointeurs;

registers registers_create() {
    registers r = malloc(sizeof(struct registers_data));
    if (r == NULL) {
        exit(1);
    }

    unsigned int i;
    unsigned int j;
    // Remplissage du cas general du tableau
    for (i = 0; i < 18; ++i) {
        for (j = 0; j < 8; ++j) {
            r->correspondance_modes[j][i] = &(r->tableau_registres[i]);
            r->correspondance_modes[j][i] = &(r->tableau_registres[i]);
        }
    }

    // Modification du tableau pour les cas particuliers
    r->correspondance_modes[0][SPSR] = NULL;
    r->correspondance_modes[1][SPSR] = NULL;

    r->correspondance_modes[2][R13] = &(r->tableau_registres[R13_SCV]);
    r->correspondance_modes[2][R14] = &(r->tableau_registres[R14_SCV]);
    r->correspondance_modes[2][SPSR] = &(r->tableau_registres[SPSR_SVC]);

    r->correspondance_modes[3][R13] = &(r->tableau_registres[R13_ABT]);
    r->correspondance_modes[3][R14] = &(r->tableau_registres[R14_ABT]);
    r->correspondance_modes[3][SPSR] = &(r->tableau_registres[SPSR_ABT]);

    r->correspondance_modes[4][R13] = &(r->tableau_registres[R13_UND]);
    r->correspondance_modes[4][R14] = &(r->tableau_registres[R14_UND]);
    r->correspondance_modes[4][SPSR] = &(r->tableau_registres[SPSR_UND]);

    r->correspondance_modes[5][R13] = &(r->tableau_registres[R13_IRQ]);
    r->correspondance_modes[5][R14] = &(r->tableau_registres[R14_IRQ]);
    r->correspondance_modes[5][SPSR] = &(r->tableau_registres[SPSR_IRQ]);

    r->correspondance_modes[6][R8] = &(r->tableau_registres[R8_FIQ]);
    r->correspondance_modes[6][R9] = &(r->tableau_registres[R9_FIQ]);
    r->correspondance_modes[6][R10] = &(r->tableau_registres[R10_FIQ]);
    r->correspondance_modes[6][R11] = &(r->tableau_registres[R11_FIQ]);
    r->correspondance_modes[6][R12] = &(r->tableau_registres[R12_FIQ]);
    r->correspondance_modes[6][R13] = &(r->tableau_registres[R13_FIQ]);
    r->correspondance_modes[6][R14] = &(r->tableau_registres[R14_FIQ]);
    r->correspondance_modes[6][SPSR] = &(r->tableau_registres[SPSR_FIQ]);

    return r;
}

void registers_destroy(registers r) {
    free(r);
    r = NULL;
}

uint8_t registers_get_mode(registers r) {
    uint32_t reg_cpsr = registers_read_cpsr(r);
    reg_cpsr &= (uint32_t) 0b11111; // Récuperation des 5 bits de mode
    //printf("%d\n",reg_cpsr);
    switch(reg_cpsr){
        case (uint32_t) 0b10000:
            return USR;
        case (uint32_t) 0b10001:
            return FIQ;
        case (uint32_t) 0b10010:
            return IRQ;
        case (uint32_t) 0b10011:
            return SVC;
        case (uint32_t) 0b10111:
            return ABT;
        case (uint32_t) 0b11011:
            return UND;
        case (uint32_t) 0b11111:
            return SYS;
        default:
            return 0;
        }
}

static int registers_mode_has_spsr(registers r, uint8_t mode) {
    return (mode == SVC || mode == ABT || mode == UND || mode == IRQ || mode == FIQ);
}

int registers_current_mode_has_spsr(registers r) {
    return registers_mode_has_spsr(r, registers_get_mode(r));
}

int registers_in_a_privileged_mode(registers r) {
    uint8_t mode = registers_get_mode(r);
    return (mode == SYS || mode == SVC || mode == ABT || mode == UND || mode == IRQ || mode == FIQ);
}

int get_mode_ligne (uint8_t mode){
    //on recupere l'index de la ligne du tableau qui correspond au mode
    switch (mode) 
    {
        case USR :
            return 0;
        case SYS :
            return 1;
        case SVC :
            return 2;
        case ABT :
            return 3;
        case UND :
            return 4;
        case IRQ :
            return 5;
        case FIQ :
            return 6;
        default :
            exit(2);
    }
}

uint32_t registers_read(registers r, uint8_t reg, uint8_t mode) {
    uint32_t value = 0;
    int ligne=get_mode_ligne(mode);  //ligne correspondant au mode

    
    if(reg > 17){
        exit(2);
    }
    if(reg == 17 && !registers_current_mode_has_spsr(r)){
        exit(2);
    }
    //on recupere la valeur du registre
    value=*(r->correspondance_modes[ligne][reg]);

    return value;
}

uint32_t registers_read_cpsr(registers r) {
    uint32_t value = 0;

    //on, renvois le registre CPSR qui est le meme pour chaque mode
    value = r->tableau_registres[CPSR];

    return value;
}

uint32_t registers_read_spsr(registers r, uint8_t mode) {
    uint32_t value = 0;

    // test si le mode a acces au registre SPSR
    if (registers_mode_has_spsr(r,mode)){
        value=*(r->correspondance_modes[get_mode_ligne(mode)][SPSR]);
    }
    else{
        //exit si le mode ne le permet pas
        exit(2);
    }

    return value;
}

void registers_write(registers r, uint8_t reg, uint8_t mode, uint32_t value) {
    //on recupere la valeur du registre
    int ligne=get_mode_ligne(mode); //ligne corespondant au mode
    if(reg > 17){
        exit(2);
    }
    if(reg == 17 && !registers_current_mode_has_spsr(r)){
        exit(2);
    }

    //on change la valeur du registre
    *(r->correspondance_modes[ligne][reg])=value;
}

void registers_write_cpsr(registers r, uint32_t value) {
    r->tableau_registres[CPSR] = value;
}

void registers_write_spsr(registers r, uint8_t mode, uint32_t value) {
    // test si le mode a acces au registre SPSR
    if (registers_mode_has_spsr(r,mode)){
        *(r->correspondance_modes[get_mode_ligne(mode)][SPSR])=value;
    }
    else{
        //exit si le mode ne le permet pas
        exit(2);
    }
}
