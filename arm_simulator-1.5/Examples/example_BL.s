.global main
.text
main:
    ldr r1, #1
incr :
    ADD r1, #1
    BL incr
.data