#include "arm_load_store.h"
#include "memory.h"
#include "registers.h"
#include "arm_core.h"
#include <stdio.h>


int main(){
    registers reg = registers_create();
    memory mem = memory_create(37);
    arm_core p = arm_create(reg,mem);

    int retour;
    uint32_t ins = 0x05801003 ;

    
    // #####IMM OFFSET
    // arm_write_register(p,0,2);
    // arm_write_register(p,1,4);

    // retour = verif_addr_mode(p,ins);
    // printf("R0 = %d\n",arm_read_register(p,0));
    // printf("R1 = %d\n",arm_read_register(p,1));
    // printf("%d\n",retour);

    // #####IMM POST
    // ins = 0x04801003 ;

    // arm_write_register(p,0,2);
    // arm_write_register(p,1,4);

    // retour = verif_addr_mode(p,ins);
    // printf("R0 = %d\n",arm_read_register(p,0));
    // printf("R1 = %d\n",arm_read_register(p,1));
    // printf("%d\n",retour);


    //######IMM PRE
    // ins = 0x051001003 ;

    // arm_write_register(p,0,2);
    // arm_write_register(p,1,4);

    // retour = verif_addr_mode(p,ins);
    // printf("R0 = %d\n",arm_read_register(p,0));
    // printf("R1 = %d\n",arm_read_register(p,1));
    // printf("%d\n",retour);

    // printf("register offset\n");
    // //#### REGISTER OFFSET
    // ins = 0x07801002 ;

    // arm_write_register(p,0,2);
    // arm_write_register(p,1,4);
    // arm_write_register(p,2,3);

    // retour = verif_addr_mode(p,ins);
    // printf("R0 = %d\n",arm_read_register(p,0));
    // printf("R1 = %d\n",arm_read_register(p,1));
    // printf("R2 = %d\n",arm_read_register(p,2));
    
    // printf("%d\n",retour);

    // // ####REGISTER PRE
    // printf("register pre_ind\n");

    // ins = 0x07A01002 ;

    // arm_write_register(p,0,2);
    // arm_write_register(p,1,4);
    // arm_write_register(p,2,3);

    // retour = verif_addr_mode(p,ins);
    // printf("R0 = %d\n",arm_read_register(p,0));
    // printf("R1 = %d\n",arm_read_register(p,1));
    // printf("R2 = %d\n",arm_read_register(p,2));
    // printf("%d\n",retour);


    // //###REGISTER POST
    // printf("register post_ind\n");

    // ins = 0x06801002 ;

    // arm_write_register(p,0,2);
    // arm_write_register(p,1,4);
    // arm_write_register(p,2,3);

    // retour = verif_addr_mode(p,ins);
    // printf("R0 = %d\n",arm_read_register(p,0));
    // printf("R1 = %d\n",arm_read_register(p,1));
    // printf("R2 = %d\n",arm_read_register(p,2));
    // printf("%d\n",retour);




    printf("scaled register offset\n");
    //#### SCALED REGISTER OFFSET
    ins = 0x07801082 ;

    arm_write_register(p,0,2);
    arm_write_register(p,1,4);
    arm_write_register(p,2,3);

    retour = verif_addr_mode(p,ins);
    printf("R0 = %d\n",arm_read_register(p,0));
    printf("R1 = %d\n",arm_read_register(p,1));
    printf("R2 = %d\n",arm_read_register(p,2));
    
    printf("%d\n",retour);

    // ####SACLED REGISTER PRE
    printf("scaled register pre_ind\n");

    ins = 0x07A01082 ;

    arm_write_register(p,0,2);
    arm_write_register(p,1,4);
    arm_write_register(p,2,3);

    retour = verif_addr_mode(p,ins);
    printf("R0 = %d\n",arm_read_register(p,0));
    printf("R1 = %d\n",arm_read_register(p,1));
    printf("R2 = %d\n",arm_read_register(p,2));
    printf("%d\n",retour);


    //###SCALED REGISTER POST
    printf("scaled register post_ind\n");

    ins = 0x06801082 ;

    arm_write_register(p,0,2);
    arm_write_register(p,1,4);
    arm_write_register(p,2,3);

    retour = verif_addr_mode(p,ins);
    printf("R0 = %d\n",arm_read_register(p,0));
    printf("R1 = %d\n",arm_read_register(p,1));
    printf("R2 = %d\n",arm_read_register(p,2));
    printf("%d\n",retour);
}