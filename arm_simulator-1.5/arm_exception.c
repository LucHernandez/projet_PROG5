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
#include "arm_exception.h"
#include "arm_constants.h"
#include "arm_core.h"
#include "registers.h"
#include "util.h"

// Not supported below ARMv6, should read as 0
#define CP15_reg1_EEbit 0
#define Exception_bit_9 (CP15_reg1_EEbit << 9)

int arm_exception(arm_core p, uint8_t exception) {
    uint32_t cpsr = 0x1d3 | Exception_bit_9;
    /* As there is no operating system in our simulator, we handle
     * software interrupts here :
     * - 0x123456 is the end of the simulation
     * - other opcodes can be used for any custom behavior,
     *   such as my_putchar given as an example
     */
    if (exception == SOFTWARE_INTERRUPT) {
        uint32_t value;
        uint32_t address = arm_read_register(p, 15);
        address -= 8;
        uint32_t instruction;
        arm_read_word(p, address, &instruction);
        instruction &= 0xFFFFFF;
        switch (instruction) {
        case 0x123456:
            return END_SIMULATION;
        case 0x000001:
            value = arm_read_register(p, 0);
            putchar(value);
            return 0;
        }
    }
    /* Aside from SWI, we only support RESET initially */
    /* Semantics of reset interrupt (ARM manual A2-18) */
    if (exception == RESET) {
        arm_write_cpsr(p, cpsr);
        arm_write_register(p, 15, 0);
    }

    //dans notre implementation les high vectors ne sont pas implementes donc pas besoin de faire le test pour chaque exception
    if (exception == UNDEFINED_INSTRUCTION){
        uint32_t cpsr_reg = arm_read_cpsr(p);
        cpsr = set_bits(cpsr_reg, 4, 0, 11011);
        cpsr = clr_bit(cpsr, 5);
        cpsr = set_bit(cpsr, 7);
        //ici les bits 6, 8 et 9 sont deja egaux a ceux du cpsr en entree donc pas de soucis de ce cote la
        arm_write_cpsr(p, cpsr);
        uint32_t pc_reg = arm_read_register(p, 15);
        arm_write_register(p, 14, pc_reg-4); //car quand on lit la pc on a ici addr instruction en cours + 8
        arm_write_spsr(p, cpsr_reg);
        arm_write_register(p, 15, 4);
    }

    if (exception == PREFETCH_ABORT){
        uint32_t cpsr_reg = arm_read_cpsr(p);
        cpsr = set_bits(cpsr_reg, 4, 0, 10111);
        cpsr = clr_bit(cpsr, 5);
        cpsr = set_bits(cpsr, 8, 7, 11);
        //ici les bits 6 et 9 sont deja egaux a ceux du cpsr en entree donc pas de soucis de ce cote la
        arm_write_cpsr(p, cpsr);
        uint32_t pc_reg = arm_read_register(p, 15);
        arm_write_register(p, 14, pc_reg); //ici pas besoin de rajouter +4 car read_register le fait deja
        arm_write_spsr(p, cpsr_reg);
        arm_write_register(p, 15, 0xC);
    }

    if (exception == DATA_ABORT){
        uint32_t cpsr_reg = arm_read_cpsr(p);
        cpsr = set_bits(cpsr_reg, 4, 0, 10111);
        cpsr = clr_bit(cpsr, 5);
        cpsr = set_bits(cpsr, 8, 7, 11);
        //ici les bits 6 et 9 sont deja egaux a ceux du cpsr en entree donc pas de soucis de ce cote la
        arm_write_cpsr(p, cpsr);
        uint32_t pc_reg = arm_read_register(p, 15);
        arm_write_register(p, 14, pc_reg); //ici on a deja l'adresse de l'instruction en cours plus 8
        arm_write_spsr(p, cpsr_reg);
        arm_write_register(p, 15, 0x10);
    }

    if (exception == INTERRUPT){
        uint32_t cpsr_reg = arm_read_cpsr(p);
        cpsr = set_bits(cpsr_reg, 4, 0, 10010);
        cpsr = clr_bit(cpsr, 5);
        cpsr = set_bits(cpsr, 8, 7, 11);
        //ici les bits 6 et 9 sont deja egaux a ceux du cpsr en entree donc pas de soucis de ce cote la
        arm_write_cpsr(p, cpsr);
        uint32_t pc_reg = arm_read_register(p, 15);
        arm_write_register(p, 14, pc_reg); //on a bien adresse de next instruction +4 car pc deja incrementee et read_register +4
        arm_write_spsr(p, cpsr_reg);
        arm_write_register(p, 15, 0x18); //la definition dans l'implementation est 0x18
    }

    if (exception == FAST_INTERRUPT){
        uint32_t cpsr_reg = arm_read_cpsr(p);
        cpsr = set_bits(cpsr_reg, 4, 0, 10001);
        cpsr = set_bits(cpsr, 8, 5, 1110);
        //ici le bit 9 est deja egal a celui du cpsr en entree donc pas de soucis de ce cote la
        arm_write_cpsr(p, cpsr);
        uint32_t pc_reg = arm_read_register(p, 15);
        arm_write_register(p, 14, pc_reg); //on a bien adresse de next instruction +4 car pc deja incrementee et read_register +4
        arm_write_spsr(p, cpsr_reg);
        arm_write_register(p, 15, 0x1C); //la definition dans l'implementation est 0x1c
    }
    return exception;
}
