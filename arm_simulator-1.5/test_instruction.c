#include "arm_instruction.h"
#include "arm_exception.h"
#include "arm_data_processing.h"
#include "arm_load_store.h"
#include "arm_branch_other.h"
#include "arm_core.h"
#include "arm_constants.h"
#include "util.h"

int test_condition(arm_core p, uint32_t reg, uint32_t ins, char* test, int res){
    arm_write_cpsr(p, reg);
    int res_trouve = arm_instruction_condition(p, ins);
    if(res_trouve != res){
        printf("Test %s : NON PASSE !!!\n", test);
        return 0;
    }
    printf("Test %s : passe\n", test);
    return 0;
}

uint32_t set_flag_cpsr(int f_z, int f_n, int f_c, int f_v){
    uint32_t reg = 0x00000000;
    if(f_z){
        reg = set_bit(reg, Z);
    }
    if(f_n){
        reg = set_bit(reg, N);
    }
    if(f_c){
        reg = set_bit(reg, C);
    }
    if(f_v){
        reg = set_bit(reg, V);
    }
    return reg;
}

int main(){
    registers reg = registers_create();
    memory mem = memory_create(100);
    arm_core core = arm_create(reg, mem);
    uint32_t cpsr_reg;
    cpsr_reg = set_flag_cpsr(1, 0, 0, 0);
    test_condition(core, cpsr_reg, 0x00000000, "Equal vrai", 0);
    cpsr_reg = set_flag_cpsr(0, 0, 0, 0);
    test_condition(core, cpsr_reg, 0x00000000, "Equal faux", -1);
    test_condition(core, cpsr_reg, 0x10000000, "Not Equal vrai", 0);
    cpsr_reg = set_flag_cpsr(1, 0, 0, 0);
    test_condition(core, cpsr_reg, 0x10000000, "Not Equal faux", -1);
    cpsr_reg = set_flag_cpsr(0, 0, 1, 0);
    test_condition(core, cpsr_reg, 0x20000000, "Carry set vrai", 0);
    cpsr_reg = set_flag_cpsr(0, 0, 0, 0);
    test_condition(core, cpsr_reg, 0x20000000, "Carry set faux", -1);
    test_condition(core, cpsr_reg, 0x30000000, "Carry clear vrai", 0);
    cpsr_reg = set_flag_cpsr(0, 0, 1, 0);
    test_condition(core, cpsr_reg, 0x30000000, "Carry clear faux", -1);
    cpsr_reg = set_flag_cpsr(0, 1, 0, 0);
    test_condition(core, cpsr_reg, 0x40000000, "Minus vrai", 0);
    cpsr_reg = set_flag_cpsr(0, 0, 0, 0);
    test_condition(core, cpsr_reg, 0x40000000, "Minus faux", -1);
    test_condition(core, cpsr_reg, 0x50000000, "Plus vrai", 0);
    cpsr_reg = set_flag_cpsr(0, 1, 0, 0);
    test_condition(core, cpsr_reg, 0x50000000, "Plus faux", -1);
    cpsr_reg = set_flag_cpsr(0, 0, 0, 1);
    test_condition(core, cpsr_reg, 0x60000000, "Overflow vrai", 0);
    cpsr_reg = set_flag_cpsr(0, 0, 0, 0);
    test_condition(core, cpsr_reg, 0x60000000, "Overflow faux", -1);
    test_condition(core, cpsr_reg, 0x70000000, "No Overflow vrai", 0);
    cpsr_reg = set_flag_cpsr(0, 0, 0, 1);
    test_condition(core, cpsr_reg, 0x70000000, "No Overflow faux", -1);
    cpsr_reg = set_flag_cpsr(0, 0, 1, 0);
    test_condition(core, cpsr_reg, 0x80000000, "Unsigned higher vrai", 0);
    cpsr_reg = set_flag_cpsr(1, 0, 0, 0);
    test_condition(core, cpsr_reg, 0x80000000, "Unsigned higher faux cas 1", -1);
    cpsr_reg = set_flag_cpsr(0, 0, 0, 0);
    test_condition(core, cpsr_reg, 0x80000000, "Unsigned higher faux cas 2", -1);
    test_condition(core, cpsr_reg, 0x90000000, "Unsigned lower vrai cas 1", 0);
    cpsr_reg = set_flag_cpsr(1, 0, 0, 0);
    test_condition(core, cpsr_reg, 0x90000000, "Unsigned lower vrai cas 2", 0);
    cpsr_reg = set_flag_cpsr(0, 0, 1, 0);
    test_condition(core, cpsr_reg, 0x90000000, "Unsigned lower faux", -1);
    cpsr_reg = set_flag_cpsr(0, 0, 0, 0);
    test_condition(core, cpsr_reg, 0xA0000000, "Signed greater than or Equal vrai cas 1", 0);
    cpsr_reg = set_flag_cpsr(0, 1, 0, 1);
    test_condition(core, cpsr_reg, 0xA0000000, "Signed greater than or Equal vrai cas 2", 0);
    cpsr_reg = set_flag_cpsr(0, 0, 0, 1);
    test_condition(core, cpsr_reg, 0xA0000000, "Signed greater than or Equal faux cas 1", -1);
    cpsr_reg = set_flag_cpsr(0, 1, 0, 0);
    test_condition(core, cpsr_reg, 0xA0000000, "Signed greater than or Equal faux cas 2", -1);
    test_condition(core, cpsr_reg, 0xB0000000, "Signed less than vrai cas 1", 0);
    cpsr_reg = set_flag_cpsr(0, 0, 0, 1);
    test_condition(core, cpsr_reg, 0xB0000000, "Signed less than vrai cas 2", 0);
    cpsr_reg = set_flag_cpsr(0, 0, 0, 0);
    test_condition(core, cpsr_reg, 0xB0000000, "Signed less than faux cas 1", -1);
    cpsr_reg = set_flag_cpsr(0, 1, 0, 1);
    test_condition(core, cpsr_reg, 0xB0000000, "Signed less than faux cas 2", -1);
    cpsr_reg = set_flag_cpsr(0, 0, 0, 0);
    test_condition(core, cpsr_reg, 0xC0000000, "Signed greater than vrai cas 1", 0);
    cpsr_reg = set_flag_cpsr(0, 1, 0, 1);
    test_condition(core, cpsr_reg, 0xC0000000, "Signed greater than vrai cas 2", 0);
    cpsr_reg = set_flag_cpsr(1, 0, 0, 0);
    test_condition(core, cpsr_reg, 0xC0000000, "Signed greater than faux cas 1", -1);
    cpsr_reg = set_flag_cpsr(0, 1, 0, 0);
    test_condition(core, cpsr_reg, 0xC0000000, "Signed greater than faux cas 2", -1);
    cpsr_reg = set_flag_cpsr(0, 0, 0, 1);
    test_condition(core, cpsr_reg, 0xC0000000, "Signed greater than faux cas 3", -1);
    test_condition(core, cpsr_reg, 0xD0000000, "Signed less than or Equal vrai cas 1", 0);
    cpsr_reg = set_flag_cpsr(0, 1, 0, 0);
    test_condition(core, cpsr_reg, 0xD0000000, "Signed less than or Equal vrai cas 2", 0);
    cpsr_reg = set_flag_cpsr(1, 0, 0, 0);
    test_condition(core, cpsr_reg, 0xD0000000, "Signed less than or Equal vrai cas 3", 0);
    cpsr_reg = set_flag_cpsr(0, 0, 0, 0);
    test_condition(core, cpsr_reg, 0xD0000000, "Signed less than or Equal faux cas 1", -1);
    cpsr_reg = set_flag_cpsr(0, 1, 0, 1);
    test_condition(core, cpsr_reg, 0xD0000000, "Signed less than or Equal faux cas 2", -1);
    test_condition(core, cpsr_reg, 0xE0000000, "Always", 0);
    test_condition(core, cpsr_reg, 0xF0000000, "Unconditional instruction", 1);
    arm_destroy(core);
    registers_destroy(reg);
    memory_destroy(mem);
    return 0;
}