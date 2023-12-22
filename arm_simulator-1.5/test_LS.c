#include "arm_load_store.h"
#include "memory.h"
#include "registers.h"
#include "arm_core.h"
#include "util.h"
#include <stdio.h>

#define ADDRESS_MODE_WB 0
#define ADDRESS_MODE_M 1
#define ADDRESS_MODE_H 1





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


void test_addr_mode_M(arm_core p,uint32_t ins){
    uint8_t RnNum = get_bits(ins,19,16);
    arm_write_register(p,0,2);
    arm_write_register(p,1,4);
    arm_write_register(p,2,3);
    uint8_t retour,i;
    uint32_t start_address=0,end_address = 0;

    uint8_t bit_P_U = get_bits(ins,24,23);
    switch (bit_P_U){
        case 0b01:
            printf("address_mode : Increment After  W = %d\n",get_bit(ins,21));
            break;
        case 0b11:
            printf("address_mode : Increment Before W = %d\n",get_bit(ins,21));
            break;
        case 0b00:
            printf("address_mode : Decrement After  W = %d\n",get_bit(ins,21));
            break;
        case 0b10:
            printf("address_mode : Decrement Before W = %d\n",get_bit(ins,21));
            break;
    }
    retour = addr_mode_M(p,ins,&start_address,&end_address);
    if (retour){
            printf("erreur de sortie de addr_mode_WB\n");
        }
    printf("Rd = R%u = %u\n",RnNum,arm_read_register(p,RnNum));
    for (i = 0; i<16;++i){
        if (get_bit(ins,i)){
            printf("R%u = %u\n",i,arm_read_register(p,i));
        }
    }
    printf("start_address = %u\n",start_address);
    printf("end_address = %u\n",end_address);
    printf("\n");
}

int main(){
    registers reg = registers_create();
    memory mem = memory_create(1024);
    arm_core p = arm_create(reg,mem);
    
    
    if (ADDRESS_MODE_WB){
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
    }

    if (ADDRESS_MODE_M){
        //Increment After
        uint32_t ins = 0x08800002; //W = 0
        test_addr_mode_M(p,ins);
        ins = 0x08A00002;          //W = 1
        test_addr_mode_M(p,ins);

        //Increment Before
        ins = 0x09800002;           //W = 0
        test_addr_mode_M(p,ins);
        ins = 0x09A00002;          //W = 1
        test_addr_mode_M(p,ins);

        //Decrement After
        ins = 0x08000002;          //W = 0
        test_addr_mode_M(p,ins);
        ins = 0x08200002;          //W = 1
        test_addr_mode_M(p,ins);

        //Decrement Before
        ins = 0x09000002;          //W = 0
        test_addr_mode_M(p,ins);
        ins = 0x09200002;          //W = 1
        test_addr_mode_M(p,ins);

    }

    registers_destroy(reg);
    memory_destroy(mem);
    arm_destroy(p);
}