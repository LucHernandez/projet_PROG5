.global main
.text
main:
    MOV r1, #0
    MOV r2, #0
    MOV r3, #1
    MOV r4, #1
    TST r1, r2
    TST r1, r3
    TST r3, r2
    TST r3, r4
.data
