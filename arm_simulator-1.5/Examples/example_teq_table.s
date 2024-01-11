.global main
.text
main:
    MOV r1, #0
    MOV r2, #0
    MOV r3, #1
    MOV r4, #1
    TEQ r1, r2
    TEQ r1, r3
    TEQ r3, r2
    TEQ r3, r4
.data
