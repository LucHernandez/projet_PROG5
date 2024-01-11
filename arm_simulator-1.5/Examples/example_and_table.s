.global main
.text
main:
    MOV r1, #0
    MOV r2, #0
    MOV r3, #1
    MOV r4, #1
    AND r5, r1, r2
    AND r5, r1, r3
    AND r5, r3, r2
    AND r5, r3, r4
.data
