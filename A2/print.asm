print:
;-----------------------------------------------------------------------------------------------------------------------------------------------------------
; The program print out the value in $1 representing it in characters
; $4 is used to store the result from slt
; $5 is set to character "-"
; $6 is set to 1
; $7 is set to -1
; $10 is set to 10
; $15 is set to be output address
; $16 is set to be newlinw in ASCII
; $28 is set to 48 for the character conversion
; $21 is the initial $1
; $22 is the initial $2
; $23 is the initial $30
; $24 is the initial $31
; $25 is the initial $1
; $26 is the initial $2

add $21, $0, $0
add $21, $21, $1
add $22, $0, $0
add $22, $22, $2
add $23, $0, $0
add $23, $23, $30
add $24, $0, $0
add $24, $24, $31
add $25, $0, $0
add $25, $25, $1
add $26, $0, $0
add $26, $26, $2


lis $28
.word 48

lis $5
.word 45
lis $6
.word 1
sub $7, $0, $6

lis $15
.word 0xffff000c ; output
; newline in ASCII
lis $16
.word 0x0a
; initialize $4 to be 0
add $4, $0, $0

; see if $1 is negative or not
slt $4, $1, $0
; print out the negative sign if $1 < 0
beq $4, $0, 3
sw $5, 0($15)
; make the number to positive number
mult $1, $7
mflo $1

; set $3 to 0
add $3, $0, $0

; set $10 to 10
lis $10
.word 10

; $2 is used to keep track of the number of digit read
; $14 is used to update the stack location address
add $2, $0, $0
lis $14
.word 4

read:
add $2, $2, $6
; get the last digit
div $1, $10
mfhi $3
mflo $1

; store the input onto the stack
sw $3, -4($30)
; update the stack starting address
sub $30, $30, $14
; check if the program read all the digit already
bne $1, $0, read

out:
add $30, $30, $14
lw $1, -4($30)
; converst the vlaue into characters
add $1, $1, $28
sw $1, 0($15)
sub $2, $2, $6
bne $2, $0, out

sw $16, 0($15)

; assign back the value
add $1, $0, $0
add $1, $1, $21
add $2, $0, $0
add $2, $2, $22
add $30, $0, $0
add $30, $30, $23
add $31, $0, $0
add $31, $31, $24
add $1, $0, $0
add $1, $1, $25
add $2, $0, $0
add $2, $2, $26


; return to the main program
jr $31
