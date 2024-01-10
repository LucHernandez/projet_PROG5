#include "arm_branch_other.h"
#include "memory.h"
#include "registers.h"
#include "arm_core.h"
#include "util.h"
#include <stdio.h>

memory m;
registers r;
arm_core p;


//test de la fonction B simple
uint32_t test_B (arm_core p, uint32_t instruction ){
    uint32_t adr=decod_adr(get_bits(instruction,23,0));
    arm_branch_other_b_bl(p,instruction);
    return adr;
}


//test de la fonction BL
uint32_t test_BL (arm_core p,uint32_t instruction){
    uint32_t adr=decod_adr(get_bits(instruction,23,0));
    arm_branch_other_b_bl(p,instruction);
    return adr;
}


//test de la fonction MRS
void test_MRS (arm_core p,uint32_t instruction){
    arm_branch_other_mrs(p,instruction);
}


void main (){
    printf("initialisation de la machine ...  ");

    //initialisation de la memoire 
    m = memory_create(32);
    r = registers_create();
    p = arm_create(r, m);

    //initialisation des instrucion de test 
    uint32_t instruction_B=0b0000101001100000001010111111000;
    uint32_t instruction_BL=0b0000101101100000001010111111000;
    uint32_t instruction_MRS_SPSR=0b0000001010000000001000000000000;
    uint32_t instruction_MRS_CPSR=0b0000001000000000001000000000000;

    printf("OK\n\n");

    //debut des test
    printf("        debut des test :\n\n");
    //test de l'instruction B
    printf("-test decodage de l'adresse...  ");
    uint32_t valeur_attendu=0b00000000110000000101011111100000;
    uint32_t adr_retour=test_B(p,instruction_B);
    if(adr_retour!=valeur_attendu){
        printf("~~Erreur~~\n\n");
    }
    else{
        printf("OK\n");
    }


    printf("-test adresse de PC...");
    
    if((adr_retour+8)!=arm_read_register(p,15)){
        printf("~~Erreur~~\n");
    }
    else{
        printf("OK\n");
    }

    //test de l'instruction BL
    printf("-test stockage adresse R14...");
    uint32_t pc_initial=arm_read_register(p,15);
    adr_retour=test_BL(p,instruction_BL);
    if(pc_initial!=arm_read_register(p,14)){
        printf("~~Erreur~~\n");
    }
    else{
        printf("OK\n");
    }

    //test copie CPSR
    printf("-test copie CPSR...");
    test_MRS(p,instruction_MRS_CPSR);
    if(arm_read_cpsr(p)!=arm_read_register(p,1)){
        printf("~~Erreur~~\n");
    }
    else{
        printf("OK\n");
    }

    //test copie SPSR
    printf("-test copie SPSR...");
    test_MRS(p,instruction_MRS_SPSR);
    if(arm_read_spsr(p)!=arm_read_register(p,1)){ 
        printf("~~Erreur~~\n");
    }
    else{
        printf("OK\n");
    }

    printf("\nfin des test\n\n");

}