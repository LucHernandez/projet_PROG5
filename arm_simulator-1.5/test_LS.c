#include "arm_load_store.h"
#include "memory.h"
#include "registers.h"
#include "arm_core.h"
#include "util.h"
#include <stdio.h>


void test_addr_mode_WB(arm_core p,uint32_t ins){
    arm_write_register(p,0,2);
    arm_write_register(p,1,4);
    arm_write_register(p,2,3);
    uint8_t retour;
    uint32_t address = 0;
    if (get_bit(ins,25) == 0){
        printf("addressing_mode : Immediate\n");

        retour = addr_mode_WB(p,ins,&address);
        if (retour){
            printf("erreur de sortie de addr_mode_WB\n");
        }
        
        printf("address = %d\n",address);
        printf("R0 = %d\n",arm_read_register(p,0));
        printf("R1 = %d\n",arm_read_register(p,1));
    }
    else if (get_bit(ins,25) && get_bits(ins,11,4) != 0){
        printf("addressing_mode : Scaled\n");
        
        retour = addr_mode_WB(p,ins,&address);
        if (retour){
            printf("erreur de sortie de addr_mode_WB\n");
        }
        printf("address = %d\n",address);
        printf("R0 = %d\n",arm_read_register(p,0));
        printf("R1 = %d\n",arm_read_register(p,1));
        printf("R2 = %d\n",arm_read_register(p,2));
    }
    else if (get_bit(ins,25) && get_bits(ins,11,4) == 0) {
        printf("addressing_mode : Register\n");

        retour = addr_mode_WB(p,ins,&address);
        if (retour){
            printf("erreur de sortie de addr_mode_WB\n");
        }
        printf("address = %d\n",address);
        printf("R0 = %d\n",arm_read_register(p,0));
        printf("R1 = %d\n",arm_read_register(p,1));
        printf("R2 = %d\n",arm_read_register(p,2));
    }
    else{
        printf("C'est pas ouf\n");
    }
    printf("\n");
}

int main(){
    registers reg = registers_create();
    memory mem = memory_create(1024);
    arm_core p = arm_create(reg,mem);
    printf("JE PUE, MERCI LUC\n");
    
    

    //addr_mode_WB
    //IMM OFFSET
    uint32_t ins = 0x05801003 ;
    test_addr_mode_WB(p,ins);
    //IMM PRE
    ins = 0x05901003 ;
    test_addr_mode_WB(p,ins);
    //IMM POST
    ins = 0x04801003 ;
    test_addr_mode_WB(p,ins);
    
    //REGISTER OFFSET
    ins = 0x07801002 ;
    test_addr_mode_WB(p,ins);
    //REGISTER PRE
    ins = 0x07A01002 ;
    test_addr_mode_WB(p,ins);
    //REGISTER POST
    ins = 0x06801002 ;
    test_addr_mode_WB(p,ins);

    //SCALED REGISTER OFFSET
    ins = 0x07801082 ;
    test_addr_mode_WB(p,ins);
    //SACLED REGISTER PRE
    ins = 0x07A01082 ;
    test_addr_mode_WB(p,ins);
    //SCALED REGISTER POST
    ins = 0x06801082 ;
    test_addr_mode_WB(p,ins);

    registers_destroy(reg);
    memory_destroy(mem);
    arm_destroy(p);
}