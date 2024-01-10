.global main
.text
main:
    MOV r1, #1
    MOV r2, #2
    ADD r1, r2
end:
    swi 0x123456
.data
