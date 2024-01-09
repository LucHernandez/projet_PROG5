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
#include "arm_branch_other.h"
#include "arm_constants.h"
#include "util.h"
#include <debug.h>
#include <stdlib.h>


int arm_branch(arm_core p, uint32_t ins) {
    int bits_25_27 = get_bits(ins, 27, 25);

    int res_instru;
    switch(bits_25_27){
        case 0b101:
            res_instru = arm_branch_other_b_bl(p, ins);
            return res_instru;
        default:
            return UNDEFINED_INSTRUCTION;
        }
}

int arm_coprocessor_others_swi(arm_core p, uint32_t ins){
    if (get_bit(ins, 24)) {
        return SOFTWARE_INTERRUPT;
    }
    return UNDEFINED_INSTRUCTION;
}

int arm_miscellaneous(arm_core p, uint32_t ins) {
    int bits_23_27 = get_bits(ins, 27, 23);
    int bits_20_21 = get_bits(ins, 21, 20);

    int res_instru;
    switch (bits_23_27)
    {
    case 0b000:
        if(bits_20_21 == 0b00){
            res_instru = arm_branch_other_mrs(p, ins);
            return res_instru;
        }
        return UNDEFINED_INSTRUCTION;
    default:
        return UNDEFINED_INSTRUCTION;
    }
}

uint32_t decod_adr (uint32_t adr){
    //ont etant le 23 eme bit 
    //si le bit est 1 alors ont set tout les bits jusqua 30 a 1
    //si non rien besoin de faire car ils sont deja egal a 0
    if(get_bit(adr,23)==1){
        adr = set_bits(adr, 29, 24, 0b111111);
    }

    //ont shifte de 2 bit pour obtenire une adresse en 32 bits
    adr=adr<<2;

    //renvois de l'adresse decoder
    return adr;
}


int arm_branch_other_b_bl(arm_core p, uint32_t ins){

    //test si le bit L (bit 24) est a un 
    if (get_bit(ins,24)==1){
        //stock l'adresse de PC dans le registre 14 aussi nomee LR
        arm_write_register(p,14,arm_read_register(p,15));
    }

    //decode le saut pour la suite des instruction a executer
    uint32_t adr=decod_adr(get_bits(ins,23,0));

    arm_write_register(p,15,arm_read_register(p,15)+adr);

    return 0;
}

int arm_branch_other_mrs (arm_core p, uint32_t ins){
    //ont recupere le numero du registre destination
    uint32_t numero_reg=get_bits(ins,15,12);

    //ont verifie que le registre destination n'est pas R15 car il est UNPREDICTABLE
    if(numero_reg==15){
        exit(1);
    }

    //si non ont continue le deroulement normal
    //ont regarde le bit 22 qui nous dit si ont veut copier CPSR ou SPSR
    if(get_bit(ins,22)==1){
        //test si le mode actuel a bien un registre spcr
        if(arm_current_mode_has_spsr(p)){
            arm_write_register(p,numero_reg,arm_read_spsr(p));
        }
        else{
            exit(2);
        }
    }
    else{
        arm_write_register(p,numero_reg,arm_read_cpsr(p));
    }
    return 0;
}


