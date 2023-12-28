#include <stdio.h>
#include <assert.h>

#include "arm_data_processing.h"
#include "util.h"
#include "registers.h"
#include "memory.h"
#include "arm_constants.h"
#include "arm_core.h"

char *operation_name[] = {"AND", "EOR", "TST", "TEQ", "CMP", "CMN", "ORR", "BIC", "SUB", "RSB", "ADD", "ADC", "SBC", "RSC", "MOV", "MVN"};

enum data_processing_operation {
    AND, EOR, TST, TEQ, CMP, CMN, ORR, BIC, 
    SUB, RSB, ADD, ADC, SBC, RSC, MOV, MVN
};

uint8_t tester (uint32_t val1, uint32_t val2, enum data_processing_operation op, uint32_t expected_flags, uint32_t expected_result) {
    memory m = memory_create(32);
    registers r = registers_create();
    arm_core p = arm_create(r, m);
    uint32_t instruction_guide = 0b00000000000100010000000000000010;
    arm_write_register(p, 1, val1);
    arm_write_register(p, 2, val2);
    switch (op) {
        case AND:
            execute_AND(p, instruction_guide);
            break;
        case EOR:
            execute_EOR(p, instruction_guide);
            break;
        case TST:
            execute_TST(p, instruction_guide);
            break;
        case TEQ:
            execute_TEQ(p, instruction_guide);
            break;
        case CMP:
            execute_CMP(p, instruction_guide);
            break;
        case CMN:
            execute_CMN(p, instruction_guide);
            break;
        case ORR:
            execute_ORR(p, instruction_guide);
            break;
        case BIC:
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
            execute_MOV(p, instruction_guide);
            break;
        case MVN:
            execute_MVN(p, instruction_guide);
            break;
    }
    uint8_t correct_results = 0;
    if (expected_result == arm_read_register(p, 0)) {
        printf("%d %s %d == %d\n", val1, operation_name[op], val2, expected_result);
        correct_results++;
    } else printf("ERROR %d %s %d = %d != %d\n", val1, operation_name[op], val2, arm_read_register(p, 0), expected_result);

    if (expected_flags >> 28 == get_bits(arm_read_cpsr(p), 32, 28)) {
        printf("NZCV %d == %d\n", get_bits(arm_read_cpsr(p), 32, 28), expected_flags >> 28);
        correct_results++;
    } else printf("NZCV %d != %d\n", get_bits(arm_read_cpsr(p), 32, 28), expected_flags >> 28);

    registers_destroy(r);
    memory_destroy(m);
    arm_destroy(p);

    printf("\n");

    return (correct_results == 2);
}

int main () {
    printf("\nTesting AND...\n");
    tester(0b01101001010, 0b01010101010, AND, 0, 0b01000001010);
    tester(0b1010101, 0, AND, (1<<Z), 0);
    tester(0xFFFFFFFF, 0xF0000000, AND, (1<<N), 0xF0000000);

    printf("\nTesting ORR...\n");
    tester(0b01101001010, 0b01010101010, ORR, 0, 0b01111101010);
    tester(0, 0, ORR, 1<<Z, 0);

    
    return 0;
}