.global main
.text
main:
    MRS r1, CPSR
end:
    swi 0x123456
.data
