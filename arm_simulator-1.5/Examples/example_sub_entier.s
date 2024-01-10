.global main
.text
main:
    MOV r1, #1
    ADD r1, #2
end:
    swi 0x123456
.data
