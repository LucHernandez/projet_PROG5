#include <stdio.h>
#include <assert.h>

#include "arm_data_processing.h"
#include "util.h"
#include "registers.h"
#include "memory.h"
#include "arm_core.h"

/*

***************************************************************

ALORS
IL EST 2H DU MATIN ET JE VIENS DE ME RENDRE COMPTE QUE JE N'AI PAS BESOIN DE TESTER LES FLAGS POUR
TOUTES LES INCTRUCTIONS ARITHMETIQUES PARCE QUE KEVIN A EU L'EXCELLENTE IDEE DE CREER UNE FONCTION
POUR FAIRE LES FLAGS
SOYEZ COMME KEVIN

***************************************************************

*/

int main() {
    memory m = memory_create(32);
    registers r = registers_create();
    arm_core p = arm_create(r, m);

    uint32_t baseI = 0b00000010000000000101000000000000;
    uint32_t basenI = 0b00000000000000000101000000000000;
    uint16_t immediate = 0b001101010101;
    uint16_t reg  = 0b000000000011;
    uint16_t lslimm = 0b000110000011;
    uint16_t lslreg = 0b000100010011;
    uint16_t lsrimm = 0b000110100010;
    uint16_t lsrreg = 0b000100110010;
    uint16_t asrimm = 0b000111000010;
    uint16_t asrreg = 0b000101010010;
    uint16_t rorimm = 0b000111100010;
    uint16_t rorreg = 0b000101110010;
    arm_write_register(p, 2, 8);
    arm_write_register(p, 1, 3);
    arm_write_register(p, 3, 10);

    // TESTS POUR VOIR SI GET_SHIFTER_OPERAND FONCTIONNE
    printf("\nEntering shifter_operand testing...\n");
    printf("Immediates: %d\n", (get_shifter_operand(p, baseI | immediate, NULL) == ror(85, 3)));
    printf("Register direct access: %d\n", (get_shifter_operand(p, basenI | reg, NULL) == 10));
    printf("Register LsL by imm: %d\n", (get_shifter_operand(p, basenI | lslimm, NULL) == 10 << 3));
    printf("Register LsL by reg: %d\n", (get_shifter_operand(p, basenI | lslreg, NULL) == 10 << 3));
    printf("Register LsR by imm: %d\n", (get_shifter_operand(p, basenI | lsrimm, NULL) == 8 >> 3));
    printf("Register LsR by reg: %d\n", (get_shifter_operand(p, basenI | lsrreg, NULL) == 8 >> 3));
    printf("Register AsR by imm: %d\n", (get_shifter_operand(p, basenI | asrimm, NULL) == 8 >> 3));
    printf("Register AsR by reg: %d\n", (get_shifter_operand(p, basenI | asrreg, NULL) == 8 >> 3));
    printf("Register RoR by imm: %d\n", (get_shifter_operand(p, basenI | rorimm, NULL) == ror(8, 3)));
    printf("Register RoR by reg: %d\n", (get_shifter_operand(p, basenI | rorreg, NULL) == ror(8, 3)));



    // TESTS POUR VOIR SI LES DIFFERENTES OPERATIONS IMPLEMENTEES FONCTIONNENT
    printf("\nEntering instruction simulation testing...\n");
    printf("\nFlag setting works the same for AND, EOR, ORR and BIC\nFlag setting works the same for TEQ and TST\nFlag setting works the same for every arithmetic operation\n");

    // TESTS AND
    printf("\nTesting AND...\n");
    ///////////////////////I////SRn  Rd  Shifter_oper
    execute_AND(p, 0b00000010000100110000000000000010);
    printf("AND returns accurate value 1 :        %d\n", arm_read_register(p, 0) == 2);
    printf("AND changes CPSR according to doc 1 : %d\n", get_bits(arm_read_cpsr(p), 31, 29) == 0b000);
    ///////////////////////I////SRn  Rd  Shifter_oper
    execute_AND(p, 0b00000010000100110000000000000101);
    printf("AND returns accurate value 2 :        %d\n", arm_read_register(p, 0) == 0);
    printf("AND changes CPSR according to doc 2 : %d\n", get_bits(arm_read_cpsr(p), 31, 29) == 0b010);
    arm_write_register(p, 0, 0xF0000000);
    ///////////////////////I////SRn  Rd  Shifter_oper
    execute_AND(p, 0b00000000000100000000000000000000);
    printf("AND returns accurate value 3 :        %d\n", arm_read_register(p, 0) == 0xF0000000);
    printf("AND changes CPSR according to doc 3 : %d\n", get_bits(arm_read_cpsr(p), 31, 29) == 0b100);

    // TESTS EOR
    printf("\nTesting EOR...\n");
    ///////////////////////I////SRn  Rd  Shifter_oper
    execute_EOR(p, 0b00000000000100000000000000000000);
    printf("EOR returns accurate value 1 :        %d\n", arm_read_register(p, 0) == 0x00000000);
    arm_write_register(p, 0, 0xF0000000);
    arm_write_register(p, 1, 0xF000000F);
    ///////////////////////I////SRn  Rd  Shifter_oper
    execute_EOR(p, 0b00000000000100000010000000000001);
    printf("EOR returns accurate value 2 :        %d\n", arm_read_register(p, 2) == 0x0000000F);

    
    // TESTS SUB
    printf("\nTesting SUB...\n");
    arm_write_register(p, 0, 0xF0000000);
    arm_write_register(p, 1, 0xF000000F);
    ///////////////////////I////SRn  Rd  Shifter_oper
    execute_SUB(p, 0b00000000000100010010000000000000);
    printf("SUB returns accurate value 1 :        %d\n", arm_read_register(p, 2) == 0x0000000F);
    printf("SUB changes CPSR according to doc 1 : %d\n", get_bits(arm_read_cpsr(p), 31, 29) == 0b000);
    arm_write_register(p, 0, 0x00000000);
    arm_write_register(p, 1, 0x0000000F);
    ///////////////////////I////SRn  Rd  Shifter_oper
    execute_SUB(p, 0b00000000000100000010000000000001);
    printf("SUB returns accurate value 2 :        %d\n", arm_read_register(p, 2) == -15);
    printf("SUB changes CPSR according to doc 2 : %d\n", get_bits(arm_read_cpsr(p), 31, 28) == 0b1011);

    // TESTS RSB
    printf("\nTesting RSB...\n");
    ///////////////////////I////SRn  Rd  Shifter_oper
    execute_RSB(p, 0b00000000000100000010000000000001);
    printf("RSB returns accurate value 1 :        %d\n", arm_read_register(p, 2) == 0x0000000F);
    printf("RSB changes CPSR according to doc 1 : %d\n", get_bits(arm_read_cpsr(p), 31, 29) == 0b000);
    ///////////////////////I////SRn  Rd  Shifter_oper
    execute_RSB(p, 0b00000000000100010010000000000000);
    printf("RSB returns accurate value 2 :        %d\n", arm_read_register(p, 2) == -15);
    printf("RSB changes CPSR according to doc 2 : %d\n", get_bits(arm_read_cpsr(p), 31, 28) == 0b1011);

    // TESTS MOV
    printf("\nTesting MOV...\n");
    ///////////////////////I////SRn  Rd  Shifter_oper
    execute_MOV(p, 0b00000000000100000010000000000001);
    printf("MOV actually moves. wow... :          %d\n", arm_read_register(p, 2) == arm_read_register(p, 1));



    
    registers_destroy(r);
    memory_destroy(m);
    arm_destroy(p);
}