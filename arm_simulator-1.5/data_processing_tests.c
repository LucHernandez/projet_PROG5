#include <stdio.h>
#include <assert.h>

#include "arm_data_processing.h"
#include "util.h"
#include "registers.h"
#include "memory.h"
#include "arm_core.h"


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
    // printf("\nEntering instruction simulation testing...\n");
    // uint32_t op;
    // for (unsigned int i = 0; i < 16;  ++i) {
    //     op = (uint32_t) i << 21;
    // }


    registers_destroy(r);
    memory_destroy(m);
    arm_destroy(p);
}