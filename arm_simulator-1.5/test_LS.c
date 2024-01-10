#include "arm_load_store.h"
#include "memory.h"
#include "registers.h"
#include "arm_core.h"
#include "util.h"
#include <stdio.h>
#include <stdlib.h>

#define ADDRESS_MODE_TEST 0
#define ADDRESS_MODE_WB 0
#define ADDRESS_MODE_M 0
#define ADDRESS_MODE_H 0

void test_addr_mode_WB(arm_core p,uint32_t ins){
    arm_write_register(p,0,2);
    arm_write_register(p,1,4);
    arm_write_register(p,2,3);
    uint8_t retour;
    uint32_t address = 0;
    if (get_bit(ins,25) == 0){
        printf("addressing_mode : Immediate\n");

        retour = Recup_addresse_WORD_BYTE_HALF(p,ins,&address,0);
        if (retour){
            printf("erreur de sortie de addr_mode_WB\n");
            exit(1);
        }
        
        printf("address = %d\n",address);
        printf("R0 = %d\n",arm_read_register(p,0));
        printf("R1 = %d\n",arm_read_register(p,1));
    }
    else if (get_bit(ins,25) && get_bits(ins,11,4) != 0){
        printf("addressing_mode : Scaled\n");
        
        retour = Recup_addresse_WORD_BYTE_HALF(p,ins,&address,0);
        if (retour){
            printf("erreur de sortie de addr_mode_WB\n");
            exit(1);
        }
        printf("address = %d\n",address);
        printf("R0 = %d\n",arm_read_register(p,0));
        printf("R1 = %d\n",arm_read_register(p,1));
        printf("R2 = %d\n",arm_read_register(p,2));
    }
    else if (get_bit(ins,25) && get_bits(ins,11,4) == 0) {
        printf("addressing_mode : Register\n");

        retour = Recup_addresse_WORD_BYTE_HALF(p,ins,&address,0);
        if (retour){
            printf("erreur de sortie de addr_mode_WB\n");
            exit(1);
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

void test_addr_mode_HALFWORD(arm_core p,uint32_t ins){
    arm_write_register(p,0,2);
    arm_write_register(p,1,4);

    uint8_t bitP = get_bit(ins,24);
    uint8_t bitW = get_bit(ins,21);
    uint8_t bit22 = get_bit(ins,22);
    if (bitP){
        if (bit22){
            if (bitW){
                printf("IMMEDIATE PRE-INDEXED\n");
            }
            else{
                printf("IMMEDIATE OFFSET\n");
            }
        }
        else{
            if (bitW){
                printf("REGISTER PRE-INDEXED\n");
            }
            else{
                printf("REGISTER OFFSET\n");
            }
        }
    }
    else {
        if (!bitW){
            if (bit22){
                printf("IMMEDIATE POST-INDEXED\n");
            }
            else{
                printf("REGISTER POST-INDEXED\n");
            }
        }
        else{
            printf("UNPREDICTABLE");
        }
    }

    uint8_t retour;
    uint32_t address = 0;
    retour = Recup_addresse_WORD_BYTE_HALF(p,ins,&address,1);
    if (retour){
        printf("erreur de sortie de addr_mode_WB\n");
        exit(1);
    }
    
    printf("address = %d\n",address);
    printf("R%d = %d\n",get_bits(ins,19,16),arm_read_register(p,0));
    printf("R%d = %d\n",get_bits(ins,15,12),arm_read_register(p,1));

}

void test_addr_mode_MULTIPLE(arm_core p,uint32_t ins){
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
    retour = addr_mode_MULTIPLE(p,ins,&start_address,&end_address);
    if (retour){
            printf("erreur de sortie de addr_mode_WB\n");
            exit(1);
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

void test_LDR_STR_et_plus(arm_core p,uint32_t ins){
    uint8_t RnNum = get_bits(ins,19,16);
    uint8_t RdNum = get_bits(ins,15,12);
    uint32_t value;
    uint32_t addr;
    uint8_t bitL = get_bit(ins,20);

    arm_write_register(p,0,4);
    arm_write_register(p,1,4);
    arm_write_register(p,2,3);
    arm_write_word(p,arm_read_register(p,RnNum),500);

    if (bitL){
        printf("avant lancement de la fonction teste :\n");
        printf("R%u = %u\n",RnNum,arm_read_register(p,RnNum));
        printf("R%u = %u\n",RdNum,arm_read_register(p,RdNum));

        arm_read_word(p,arm_read_register(p,RnNum),&value);
        printf("valeur dans la memoire avant %u\n\n",value);

        arm_load_store(p,ins);

        Recup_addresse_WORD_BYTE_HALF(p,ins,&addr,0);

        printf("apres lancement de la fonction teste :\n");
        printf("R%u = %u\n",RnNum,arm_read_register(p,RnNum));
        printf("R%u = %u\n",RdNum,arm_read_register(p,RdNum));
        
        arm_read_word(p,addr,&value);
        printf("valeur dans la memoire apres %u\n",value);
    }
    else{
        printf("avant lancement de la fonction teste :\n");
        printf("R%u = %u\n",RnNum,arm_read_register(p,RnNum));
        printf("R%u = %u\n",RdNum,arm_read_register(p,RdNum));

        arm_read_word(p,arm_read_register(p,RnNum),&value);
        printf("valeur dans la memoire avant %u\n\n",value);

        arm_load_store(p,ins);

        printf("apres lancement de la fonction teste :\n");
        printf("R%u = %u\n",RnNum,arm_read_register(p,RnNum));
        printf("R%u = %u\n",RdNum,arm_read_register(p,RdNum));
        
        Recup_addresse_WORD_BYTE_HALF(p,ins,&addr,0);

        arm_read_word(p,addr,&value);
        printf("valeur dans la memoire apres %u\n",value);
    }
    printf("\n\n");
}

void test_addr_mode(arm_core p){
    
    if (ADDRESS_MODE_WB){

        printf("test adressing mode : Word et Byte\n\n");
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

    if (ADDRESS_MODE_H){
        printf("test adressing mode : Halfword\n\n");
        //IMMEDIATE PRE_INDEXED
        uint32_t ins = 0x01E012F1;
        test_addr_mode_HALFWORD(p,ins);
        //IMMEDDIATE OFFSET
        ins = 0x01C012F1;
        test_addr_mode_HALFWORD(p,ins);
        //IMMEDDIATE POST-INDEXED
        ins = 0x00C012F1;
        test_addr_mode_HALFWORD(p,ins);


        //REGISTER PRE-INDEXED
        ins = 0x01A012F1;
        test_addr_mode_HALFWORD(p,ins);
        //REGISTER OFFSET
        ins = 0x018012F1;
        test_addr_mode_HALFWORD(p,ins);
        //REGISTER POST-INDEXED
        ins = 0x008012F1;
        test_addr_mode_HALFWORD(p,ins);
    }

    if (ADDRESS_MODE_M){
        printf("test adressing mode : Multiple \n\n");
        //Increment After
        uint32_t ins = 0x08800002; //W = 0
        test_addr_mode_MULTIPLE(p,ins);
        ins = 0x08A00002;          //W = 1
        test_addr_mode_MULTIPLE(p,ins);

        //Increment Before
        ins = 0x09800002;           //W = 0
        test_addr_mode_MULTIPLE(p,ins);
        ins = 0x09A00002;          //W = 1
        test_addr_mode_MULTIPLE(p,ins);

        //Decrement After
        ins = 0x08000002;          //W = 0
        test_addr_mode_MULTIPLE(p,ins);
        ins = 0x08200002;          //W = 1
        test_addr_mode_MULTIPLE(p,ins);

        //Decrement Before
        ins = 0x09000002;          //W = 0
        test_addr_mode_MULTIPLE(p,ins);
        ins = 0x09200002;          //W = 1
        test_addr_mode_MULTIPLE(p,ins);

    }
}

int main(){
    registers reg = registers_create();
    memory mem = memory_create(1024);
    arm_core p = arm_create(reg,mem);

    if (ADDRESS_MODE_TEST) test_addr_mode(p);

    uint32_t ins = 0x07901000; //LDR
    test_LDR_STR_et_plus(p,ins);
    //0000 0101 1001 0000 0001 0000 0000 0000
    ins = 0x07801000; //LDR
    test_LDR_STR_et_plus(p,ins);

    registers_destroy(reg);
    memory_destroy(mem);
    arm_destroy(p);
}