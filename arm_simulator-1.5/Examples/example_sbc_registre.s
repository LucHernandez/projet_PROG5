.global main
.text
main:
    MOV r1, #1
    MOV r2, #2
    SBC r1, r2
    swi 0x123456
.data
