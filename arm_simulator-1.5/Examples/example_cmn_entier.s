.global main
.text
main:
    MOV r1, #1
    CMN r1, #2
    swi 0x123456
.data
