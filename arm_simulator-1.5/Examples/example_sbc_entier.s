.global main
.text
main:
    MOV r1, #1
    SBC r1, #2
    swi 0x123456
.data
