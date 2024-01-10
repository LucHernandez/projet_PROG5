#include <stdio.h>
#include <assert.h>

#include "arm_data_processing.h"
#include "util.h"
#include "registers.h"
#include "memory.h"
#include "arm_constants.h"
#include "arm_core.h"

char *operation_name[] = {"AND", "EOR", "TST", "TEQ", "CMP", "CMN", "ORR", "BIC", "SUB", "RSB", "ADD", "ADC", "SBC", "RSC", "MOV", "MVN"};
memory m;
registers r;
arm_core p;

enum data_processing_operation {
    AND, EOR, TST, TEQ, CMP, CMN, ORR, BIC, 
    SUB, RSB, ADD, ADC, SBC, RSC, MOV, MVN
};

uint8_t tester (uint32_t val1, uint32_t val2, enum data_processing_operation op, uint32_t expected_flags, uint32_t expected_result) {
    // Recuperation du Carry precedent pour le transmettre et mettre a jour l'expected flag
    uint32_t former_carry = get_bit(arm_read_cpsr(p), C) << C;
    uint32_t instruction_guide = 0b00000000000100010000000000000010;
    arm_write_register(p, 1, val1);
    arm_write_register(p, 2, val2);
    switch (op) {
        case AND:
            expected_flags |= former_carry;
            execute_AND(p, instruction_guide);
            break;
        case EOR:
            expected_flags |= former_carry;
            execute_EOR(p, instruction_guide);
            break;
        case TST:
            expected_flags |= former_carry;
            execute_TST(p, instruction_guide);
            break;
        case TEQ:
            expected_flags |= former_carry;
            execute_TEQ(p, instruction_guide);
            break;
        case CMP:
            execute_CMP(p, instruction_guide);
            break;
        case CMN:
            execute_CMN(p, instruction_guide);
            break;
        case ORR:
            expected_flags |= former_carry;
            execute_ORR(p, instruction_guide);
            break;
        case BIC:
            expected_flags |= former_carry;
            execute_BIC(p, instruction_guide);
            break;
        case SUB:
            execute_SUB(p, instruction_guide);
            break;
        case RSB:
            execute_RSB(p, instruction_guide);
            break;
        case ADD:
            execute_ADD(p, instruction_guide);
            break;
        case ADC:
            execute_ADC(p, instruction_guide);
            break;
        case SBC:
            execute_SBC(p, instruction_guide);
            break;
        case RSC:
            execute_RSC(p, instruction_guide);
            break;
        case MOV:
            expected_flags |= former_carry;
            execute_MOV(p, instruction_guide);
            break;
        case MVN:
            expected_flags |= former_carry;
            execute_MVN(p, instruction_guide);
            break;
    }
    uint8_t correct_results = 0;
    if (expected_result == arm_read_register(p, 0)) {
        printf("%d %s %d == %d\n", val1, operation_name[op], val2, expected_result);
        correct_results++;
    } else printf("ERROR %d %s %d = %d != %d\n", val1, operation_name[op], val2, arm_read_register(p, 0), expected_result);

    if (expected_flags >> 28 == get_bits(arm_read_cpsr(p), 31, 28)) {
        printf("NZCV %d == %d\n", get_bits(arm_read_cpsr(p), 31, 28), expected_flags >> 28);
        correct_results++;
    } else printf("NZCV %d != %d\n", get_bits(arm_read_cpsr(p), 31, 28), expected_flags >> 28);

    printf("\n");

    return (correct_results == 2);
}

int main () {
    m = memory_create(32);
    r = registers_create();
    p = arm_create(r, m);

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

    // printf("\nTesting AND...\n");
    // tester(0b01101001010, 0b01010101010, AND, 0, 0b01000001010);
    // tester(0b1010101, 0, AND, (1<<Z), 0);
    // tester(0xFFFFFFFF, 0xF0000000, AND, (1<<N), 0xF0000000);

    // printf("\nTesting ORR...\n");
    // tester(0b01101001010, 0b01010101010, ORR, 0, 0b01111101010);
    // tester(0, 0, ORR, 1<<Z, 0);
    // tester(0xFFFFFFFF, 0, ORR, (1<<N), 0xFFFFFFFF);

    // printf("\nTesting ADD...\n");
    // tester(5, 7, ADD, 0, 12);
    // tester(0xFFFFFFFF, 1, ADD, (1<<Z) | (1<<C), 0);
    // tester(0x7FFFFFFF, 1, ADD, (1<<N) | (1<<V), 0x80000000);
    // tester(0xFFFFFFFF, 0x80000000, ADD, (1<<V)| (1<<C), 0x7FFFFFFF);

    // printf("\nTesting ADC...\n");
    // tester(5, 7, ADC, 0, 12);
    // tester(0xFFFFFFFF, 1, ADC, (1<<Z) | (1<<C), 0);
    // tester(0x7FFFFFFF, 1, ADC, (1<<N) | (1<<V), 0x80000001);
    // tester(0xFFFFFFFF, 0x80000000, ADC, (1<<V)| (1<<C), 0x7FFFFFFF);

    // printf("\nTesting CMN...\n");
    // tester(5, 7, CMN, 0, 0x7FFFFFFF);
    // tester(0xFFFFFFFF, 1, CMN, (1<<Z) | (1<<C), 0x7FFFFFFF);
    // tester(0x7FFFFFFF, 1, CMN, (1<<N) | (1<<V), 0x7FFFFFFF);
    // tester(0xFFFFFFFF, 0x80000000, CMN, (1<<V)| (1<<C), 0x7FFFFFFF);

    // printf("\nTesting EOR...\n");
    // tester(0b01101001010, 0b01010101010, EOR, 0, 0b00111100000);
    // tester(0x80000000, 0, EOR, (1<<N), 0x80000000);
    // tester(0, 0, EOR, (1<<Z), 0);

    // printf("\nTesting MOV...\n");
    // tester(0, 0xABCDEF, MOV, 0, 0xABCDEF);
    // tester(0, 0xFFFFFFFF, MOV, (1<<N), 0xFFFFFFFF);
    // tester(0, 0, MOV, (1<<Z), 0);

    // printf("\nTesting BIC...\n");
    // tester(0xFFFFFFFF, 0xF0000000, BIC, 0, 0x0FFFFFFF);
    // tester(0X80000000, 0x7FFFFFFF, BIC, (1<<N), 0x80000000);
    // tester(0xFFFFFFFF, 0xFFFFFFFF, BIC, (1<<Z), 0);

    // printf("\nTesting MVN...\n");
    // tester(0, 0xFFFFFCB5,  MVN, 0, 0b01101001010);
    // tester(0, 0, MVN, (1<<N), 0xFFFFFFFF);
    // tester(0, 0xFFFFFFFF, MVN, (1<<Z), 0);


    // printf("\nTesting TST...\n");
    // tester(0b01101001010, 0b01010101010, TST, 0, 0);
    // tester(0xFFFFFFFF, 0xF0000000, TST, (1<<N), 0);
    // tester(0xFFFFFFFF, 0, TST, (1<<Z), 0);

    // printf("\nTesting TEQ...\n");
    // tester(0b01101001010, 0b01010101010, TEQ, 0, 0);
    // tester(0x00000000, 0xF0000000, TEQ, (1<<N), 0);
    // tester(0, 0, TEQ, (1<<Z), 0);

    printf("\nTesting SUB...\n");
    tester(0xF0000000, 0x7FFFFFFF, SUB, (1<<V) | (1<<C), 0x70000001);
    tester(0xF, 0xF, SUB, (1<<Z) | (1<<C), 0);
    tester(7, 5, SUB, (1<<C), 2);
    tester(1, 2, SUB, (1<<N), 0xFFFFFFFF);

    printf("\nTesting RSB...\n");
    tester(0x7FFFFFFF, 0xF0000000, RSB, (1<<V) | (1<<C), 0x70000001);
    tester(0xF, 0xF, RSB, (1<<Z) | (1<<C), 0);
    tester(5, 7, RSB, (1<<C), 2);
    tester(2, 1, RSB, (1<<N), 0xFFFFFFFF);

    // printf("\nTesting SBC...\n");
    // tester(5, 7, SBC, (1<<N), -3);
    // tester(0, 1, SBC, (1<<N), -2);

    // printf("\nTesting RSC...\n");
    // tester(5, 7, RSC, (1<<C), 1);
    // tester(0, 1, RSC, (1<<C), 1);

    // printf("\nTesting CMP...\n");
    // tester(7, 5, CMP, (1<<C), 1);
    // tester(1, 2, CMP, (1<<N), 1);
    // tester(2, 2, CMP, (1<<Z) | (1<<C), 1);


    registers_destroy(r);
    memory_destroy(m);
    arm_destroy(p);

    return 0;
}