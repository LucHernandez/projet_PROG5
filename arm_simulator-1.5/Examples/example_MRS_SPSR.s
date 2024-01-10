.global main
.text
main:
    MRS r1, SPSR
end:
    swi 0x123456
.data
