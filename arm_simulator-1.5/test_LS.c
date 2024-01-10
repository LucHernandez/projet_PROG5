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

#define TEST_LDR 0
#define TEST_LDRB 0
#define TEST_LDRH 1

#define TEST_STR 0
#define TEST_STRB 0
#define TEST_STRH 0

#define TEST_IMMEDIATE 1
#define TEST_REGISTER 1
#define TEST_SCALED_REGISTER 0

#define TEST_LDM 0
#define TEST_STM 0

#define TEST_INCREMENT_BEFORE 1
#define TEST_INCREMENT_AFTER 1
#define TEST_DECREMENT_BEFORE 1
#define TEST_DECREMENT_AFTER 1


//##################    ADRESSING MODE    ####################
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

void test_addr_mode(arm_core p){
    
    if (ADDRESS_MODE_WB){

        printf("test adressing mode : Word et Byte\n\n");
        //addr_mode_WB
        //IMM OFFSET
        uint32_t ins = 0x05801003 ;
        test_addr_mode_WB(p,ins);
        //IMM PRE
        ins = 0x05A01003 ;
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


//######## STR STRB STRH || LDR LDRB LDRH###################
void test_LDR_STR_et_plus(arm_core p,uint32_t ins,uint8_t flag_Half){
    uint8_t RnNum = get_bits(ins,19,16);
    uint8_t RdNum = get_bits(ins,15,12);
    
    uint32_t addr;
    uint8_t bitL = get_bit(ins,20);
    uint16_t valueh;
    uint8_t valueb;
    uint32_t value;
    arm_write_register(p,0,12);
    arm_write_register(p,1,8);
    arm_write_register(p,2,4);
    
    Recup_addresse_WORD_BYTE_HALF(p,ins,&addr,flag_Half);
    
    arm_write_register(p,0,12);
    arm_write_register(p,1,8);
    arm_write_register(p,2,4);

    arm_write_word(p,addr,4);

    uint8_t bitB = get_bit(ins,22);


    if (bitL){
        printf("avant lancement de la fonction teste :\n");
        printf("R%u = %u\n",RnNum,arm_read_register(p,RnNum));
        printf("R%u = %u\n",RdNum,arm_read_register(p,RdNum));
        
        if (flag_Half){
            arm_read_half(p,addr,&valueh);
            printf("valeur dans la memoire[%u] avant %u\n\n",addr,valueh);
        }
        else{
            if (bitB) {
                arm_read_byte(p,addr,&valueb);
                printf("valeur dans la memoire[%u] avant %u\n\n",addr,valueb);
                }
            else {
                arm_read_word(p,addr,&value);
                printf("valeur dans la memoire[%u] avant %u\n\n",addr,value);
            }
        }

        arm_load_store(p,ins);

        printf("apres lancement de la fonction teste :\n");
        printf("R%u = %u\n",RnNum,arm_read_register(p,RnNum));
        printf("R%u = %u\n",RdNum,arm_read_register(p,RdNum));
        
        if (flag_Half){
            arm_read_half(p,addr,&valueh);
            printf("valeur dans la memoire[%u] avant %u\n\n",addr,valueh);
        }
        else{
            if (bitB) {
                arm_read_byte(p,addr,&valueb);
                printf("valeur dans la memoire[%u] avant %u\n\n",addr,valueb);
                }
            else {
                arm_read_word(p,addr,&value);
                printf("valeur dans la memoire[%u] avant %u\n\n",addr,value);
            }
        }
    }
    else{
        printf("avant lancement de la fonction teste :\n");
        printf("R%u = %u\n",RnNum,arm_read_register(p,RnNum));
        printf("R%u = %u\n",RdNum,arm_read_register(p,RdNum));
        
                if (flag_Half){
            arm_read_half(p,addr,&valueh);
            printf("valeur dans la memoire[%u] avant %u\n\n",addr,valueh);
        }
        else{
            if (bitB) {
                arm_read_byte(p,addr,&valueb);
                printf("valeur dans la memoire[%u] avant %u\n\n",addr,valueb);
                }
            else {
                arm_read_word(p,addr,&value);
                printf("valeur dans la memoire[%u] avant %u\n\n",addr,value);
            }
        }

        arm_load_store(p,ins);

        printf("apres lancement de la fonction teste :\n");
        printf("R%u = %u\n",RnNum,arm_read_register(p,RnNum));
        printf("R%u = %u\n",RdNum,arm_read_register(p,RdNum));

        if (flag_Half){
            arm_read_half(p,addr,&valueh);
            printf("valeur dans la memoire[%u] avant %u\n\n",addr,valueh);
        }
        else{
            if (bitB) {
                arm_read_byte(p,addr,&valueb);
                printf("valeur dans la memoire[%u] avant %u\n\n",addr,valueb);
                }
            else {
                arm_read_word(p,addr,&value);
                printf("valeur dans la memoire[%u] avant %u\n\n",addr,value);
            }
        }
    }
    printf("\n\n");
    }


void instruction_str(arm_core p){
    printf("STR\n");
    if (TEST_IMMEDIATE){
        printf("IMM OFFSET\n");
        uint32_t ins = 0x05801002 ;
        test_LDR_STR_et_plus(p,ins,0);
        printf("IMM PRE-INDEXED\n");
        ins = 0x05A01002 ;
        test_LDR_STR_et_plus(p,ins,0);
        printf("IMM POST-INDEXED\n");
        ins = 0x04801002 ;
        test_LDR_STR_et_plus(p,ins,0);
    }
    if (TEST_REGISTER){
        printf("REGISTER OFFSET\n");
        uint32_t ins = 0x07801002 ;
        test_LDR_STR_et_plus(p,ins,0);
        printf("REGISTER PRE-INDEXED\n");
        ins = 0x07A01002 ;
        test_LDR_STR_et_plus(p,ins,0);
        printf("REGISTER POST-INDEXED\n");
        ins = 0x06801002 ;
        test_LDR_STR_et_plus(p,ins,0);
    }

    if (TEST_SCALED_REGISTER){
        printf("SCALED REGISTER OFFSET\n");
        uint32_t ins = 0x07801082 ;
        test_LDR_STR_et_plus(p,ins,0);
        printf("SACLED REGISTER PRE-INDEXED\n");
        ins = 0x07A01082 ;
        test_LDR_STR_et_plus(p,ins,0);
        printf("SCALED REGISTER POST-INDEXED\n");
        ins = 0x06801082 ;
        test_LDR_STR_et_plus(p,ins,0);
    }
}

void instruction_strb(arm_core p){
    printf("STRB\n");
    if (TEST_IMMEDIATE){
        uint32_t ins = 0x05C01003 ;
        test_LDR_STR_et_plus(p,ins,0);
        printf("IMM PRE-INDEXED\n");
        ins = 0x05E01003;
        test_LDR_STR_et_plus(p,ins,0);
        printf("IMM POST-INDEXED\n");
        ins = 0x04C01003 ;
        test_LDR_STR_et_plus(p,ins,0);
    }
    if (TEST_REGISTER){
        printf("REGISTER OFFSET\n");
        uint32_t ins = 0x07C01002 ;
        test_LDR_STR_et_plus(p,ins,0);
        printf("REGISTER PRE-INDEXED\n");
        ins = 0x07E01002 ;
        test_LDR_STR_et_plus(p,ins,0);
        printf("REGISTER POST-INDEXED\n");
        ins = 0x06C01002 ;
        test_LDR_STR_et_plus(p,ins,0);
    }

    if (TEST_SCALED_REGISTER){
        printf("SCALED REGISTER OFFSET\n");
        uint32_t ins = 0x07C01082 ;
        test_LDR_STR_et_plus(p,ins,0);
        printf("SACLED REGISTER PRE-INDEXED\n");
        ins = 0x07E01082 ;
        test_LDR_STR_et_plus(p,ins,0);
        printf("SCALED REGISTER POST-INDEXED\n");
        ins = 0x06C01082 ;
        test_LDR_STR_et_plus(p,ins,0);
    }
}

void instruction_strh(arm_core p){
    printf("STRH\n");
    if (TEST_IMMEDIATE){
        printf("IMMEDIATE PRE_INDEXED\n");
        uint32_t ins = 0x01E010B0;
        //0000 0001 1110 0000 0001 0010 1011 0001
        test_LDR_STR_et_plus(p,ins,1);
        printf("IMMEDDIATE OFFSET\n");
        ins = 0x01C010B0;
        //0000 0001 1100 0000 0001 0010 1011 0001
        test_LDR_STR_et_plus(p,ins,1);
        printf("IMMEDDIATE POST-INDEXED\n");
        ins = 0x00C010B0;
        //0000 0000 1100 0000 0001 0010 1011 0001
        test_LDR_STR_et_plus(p,ins,1);
    }
    if (TEST_REGISTER){
        printf("REGISTER PRE-INDEXED\n");
        uint32_t ins = 0x01A012B1;
        //0000 0001 1010 0000 0001 0010 1011 0001
        test_LDR_STR_et_plus(p,ins,1);
        printf("REGISTER OFFSET\n");
        ins = 0x018010B1;
        //0000 0001 1000 0000 0001 0000 1011 0001
        test_LDR_STR_et_plus(p,ins,1);
        printf("REGISTER POST-INDEXED\n");
        ins = 0x008010B1;
        //0000 0000 1000 0000 0001 0000 1011 0001
        test_LDR_STR_et_plus(p,ins,1);
    }
}

void instruction_ldr(arm_core p){
    printf("LDR\n");
    if (TEST_IMMEDIATE){
        printf("IMM OFFSET\n");
        uint32_t ins = 0x05901004;
        //0000 0101 1001 0000 0001 0000 0000 
        test_LDR_STR_et_plus(p,ins,0);
        printf("IMM PRE-INDEXED\n");
        ins = 0x05B01004;
        test_LDR_STR_et_plus(p,ins,0);
        printf("IMM POST-INDEXED\n");
        ins = 0x04901004;
        test_LDR_STR_et_plus(p,ins,0);
    }
    if (TEST_REGISTER){
        printf("REGISTER OFFSET\n");
        uint32_t ins = 0x07901002 ;
        test_LDR_STR_et_plus(p,ins,0);
        printf("REGISTER PRE-INDEXED\n");
        ins = 0x07B01002 ;
        test_LDR_STR_et_plus(p,ins,0);
        printf("REGISTER POST-INDEXED\n");
        ins = 0x06901002 ;
        test_LDR_STR_et_plus(p,ins,0);
    }
    if (TEST_SCALED_REGISTER){
        printf("SCALED REGISTER OFFSET\n");
        uint32_t ins = 0x07901082 ;
        test_LDR_STR_et_plus(p,ins,0);
        printf("SCALED REGISTER PRE-INDEXED\n");
        ins = 0x07B01082 ;
        test_LDR_STR_et_plus(p,ins,0);
        printf("SCALED REGISTER POST-INDEXED\n");
        ins = 0x06901082 ;
        test_LDR_STR_et_plus(p,ins,0);
    }
}

void instruction_ldrb(arm_core p){
    printf("LDRB\n");
    if (TEST_IMMEDIATE){
        uint32_t ins = 0x05D01003 ;
        test_LDR_STR_et_plus(p,ins,0);
        printf("IMM PRE-INDEXED\n");
        ins = 0x05F01003 ;
        test_LDR_STR_et_plus(p,ins,0);
        printf("IMM POST-INDEXED\n");
        ins = 0x04D01003 ;
        test_LDR_STR_et_plus(p,ins,0);
    }
    if (TEST_REGISTER){
        printf("REGISTER OFFSET\n");
        uint32_t ins = 0x07D01002 ;
        test_LDR_STR_et_plus(p,ins,0);
        printf("REGISTER PRE-INDEXED\n");
        ins = 0x07F01002 ;
        test_LDR_STR_et_plus(p,ins,0);
        printf("REGISTER POST-INDEXED\n");
        ins = 0x06D01002 ;
        test_LDR_STR_et_plus(p,ins,0);
    }
    if (TEST_SCALED_REGISTER){
        printf("SCALED REGISTER OFFSET\n");
        uint32_t ins = 0x07D01082 ;
        test_LDR_STR_et_plus(p,ins,0);
        printf("SCALED REGISTER PRE-INDEXED\n");
        ins = 0x07F01082 ;
        test_LDR_STR_et_plus(p,ins,0);
        printf("SCALED REGISTER POST-INDEXED\n");
        ins = 0x06D01082 ;
        test_LDR_STR_et_plus(p,ins,0);
    }
}

void instruction_ldrh(arm_core p){
    printf("LDRH\n");
    if (TEST_IMMEDIATE){
        printf("IMMEDIATE PRE_INDEXED\n");
        uint32_t ins = 0x01F010B4;
        test_LDR_STR_et_plus(p,ins,1);
        printf("IMMEDDIATE OFFSET\n");
        ins = 0x01D010B4;
        test_LDR_STR_et_plus(p,ins,1);
        printf("IMMEDDIATE POST-INDEXED\n");
        ins = 0x00D010B4;
        test_LDR_STR_et_plus(p,ins,1);
    }
    if (TEST_REGISTER){
        printf("REGISTER PRE-INDEXED\n");
        uint32_t ins = 0x01B012B1;
        test_LDR_STR_et_plus(p,ins,1);
        printf("REGISTER OFFSET\n");
        ins = 0x019010B1;
        test_LDR_STR_et_plus(p,ins,1);
        printf("REGISTER POST-INDEXED\n");
        ins = 0x009010B1;
        test_LDR_STR_et_plus(p,ins,1);
    }
}



//################## STM || LDM #####################
void test_LDM_STM(arm_core p,uint32_t ins){
    arm_write_register(p,0,12);
    arm_write_register(p,1,8);
    arm_write_register(p,2,4);
    uint8_t i;
    uint8_t RnNum = get_bits(ins,19,16);
    uint32_t valreg;
    uint32_t value;
    uint32_t addr = 0;
    uint32_t end_address = 0;

    addr_mode_MULTIPLE(p,ins,&addr,&end_address);

    uint32_t addr2 = addr;

    arm_write_register(p,0,12);
    arm_write_register(p,1,8);
    arm_write_register(p,2,4);

    for (i=0;i<16;++i){
        if (get_bit(ins,i)){
            valreg = arm_read_register(p,i);
            printf("R%u = %u\n",i,valreg);
            arm_read_word(p,addr,&value);
            printf("Memory[%u] = %u\n",addr,value);
            addr+=4;
        }
    }
    printf("Rn = R%u = %u\n",RnNum,arm_read_register(p,RnNum));

    arm_load_store_multiple(p,ins);

    for (i=0;i<16;++i){
        if (get_bit(ins,i)){
            valreg = arm_read_register(p,i);
            printf("R%u = %u\n",i,valreg);
            arm_read_word(p,addr2,&value);
            printf("Memory[%u] = %u\n",addr2,value);
            addr2+=4;
        }
    }
    printf("\n\n");   
}

void instruction_stm(arm_core p){
    printf("STM\n");
    if (TEST_INCREMENT_AFTER){
        printf("Increment After");
        uint32_t ins = 0x08800004; //W = 0
        test_LDM_STM(p,ins);
        ins = 0x08A00004;          //W = 1
        test_LDM_STM(p,ins);
    }
    if (TEST_INCREMENT_BEFORE){
        printf("Increment Before");
        uint32_t ins = 0x09800004;           //W = 0
        test_LDM_STM(p,ins);
        ins = 0x09A00004;          //W = 1
        test_LDM_STM(p,ins);
    }
    if (TEST_DECREMENT_AFTER){
        printf("Decrement After");
        uint32_t ins = 0x08000004;          //W = 0
        test_LDM_STM(p,ins);
        ins = 0x08200004;          //W = 1
        test_LDM_STM(p,ins);
    }
    if (TEST_DECREMENT_BEFORE){
        printf("Decrement Before");
        uint32_t ins = 0x09000004;          //W = 0
        test_LDM_STM(p,ins);
        ins = 0x09200004;          //W = 1
        test_LDM_STM(p,ins);
    }
}

void instruction_ldm(arm_core p){
    printf("LDM\n");
    if (TEST_INCREMENT_AFTER){
        printf("Increment After");
        uint32_t ins = 0x08900004; //W = 0
        test_LDM_STM(p,ins);
        ins = 0x08B00004;          //W = 1
        test_LDM_STM(p,ins);
    }
    if (TEST_INCREMENT_BEFORE){
        printf("Increment Before");
        uint32_t ins = 0x09900004;           //W = 0
        test_LDM_STM(p,ins);
        ins = 0x09B00004;          //W = 1
        test_LDM_STM(p,ins);
    }
    if (TEST_DECREMENT_AFTER){
        printf("Decrement After");
        uint32_t ins = 0x08100004;          //W = 0
        test_LDM_STM(p,ins);
        ins = 0x08300004;          //W = 1
        test_LDM_STM(p,ins);
    }
    if (TEST_DECREMENT_BEFORE){
        printf("Decrement Before");
        uint32_t ins = 0x09100004;          //W = 0
        test_LDM_STM(p,ins);
        ins = 0x09300004;          //W = 1
        test_LDM_STM(p,ins);
    }
}



//################# main
int main(){
    registers reg = registers_create();
    memory mem = memory_create(8192);
    arm_core p = arm_create(reg,mem);

    if (ADDRESS_MODE_TEST) test_addr_mode(p);

    if (TEST_LDR) instruction_ldr(p);
    if (TEST_LDRB) instruction_ldrb(p);
    if (TEST_LDRH) instruction_ldrh(p);

    if (TEST_STR) instruction_str(p);
    if (TEST_STRB) instruction_strb(p);
    if (TEST_STRH) instruction_strh(p);

    if (TEST_LDM) instruction_ldm(p);
    if (TEST_STM) instruction_stm(p);

    registers_destroy(reg);
    memory_destroy(mem);
    arm_destroy(p);
}