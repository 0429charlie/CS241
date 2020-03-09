; The program reads an array and return the median
; The length of the aray need to be odd


; $16 is used for division by 2
lis $16
.word 2

; $17 is used for addition by 1
lis $17
.word 1

; If there is only one element in the array
bne $2, $17, 2 
lw $3, 0($1)
jr $31

; $14 is used to update the address, always set to 4
lis $14
.word 4

; $15 is used to jump to the function split
lis $15
.word split

; $19 is use to jump to the function merge
lis $19
.word merge

; save $1, $2, $31 onto the stack
sw $1, -4($30)
sub $30, $30, $14
sw $2, -4($30)
sub $30, $30, $14
sw $31, -4($30)
sub $30, $30, $14

; call split
jalr $15

; restore $1, $2, $31
add $30, $30, $14
lw $31, -4($30)
add $30, $30, $14
lw $2, -4($30)
add $30, $30, $14
lw $1, -4($30)


;return the program
divu $2, $16
mflo $2
add $2, $2, $17
multu $2, $14
mflo $2
add $1, $1, $2
lw $3, 0($1)
jr $31


; $18 is use for slt comparison
split:
slt $18, $2, $16
beq $18, $0, 6
sw $31, -4($30)
sub $30, $30, $14
jalr $19
add $30, $30, $14
lw $31, -4($30)
jr $31

divu $2, $16 ; Length 1
mflo $4
add $5, $0, $0 ; address 1
add $5, $5, $1
sub $6, $2, $4 ; Length 2
multu $4, $14
mflo $8
add $7, $0, $0
add $7, $1, $8 ; address 2

; save $2 and $1 to the stack
sw $1, -4($30)
sub $30, $30, $14
sw $2, -4($30)
sub $30, $30, $14
sw $31, -4($30)
sub $30, $30, $14

; update $1 and $2
add $2, $0, $0
add $2, $2, $4
add $1, $0, $0
add $1, $1, $5

; recursive calling split
jalr $15

; save $4, $5, $6, $7 to tack
sw $4, -4($30)
sub $30, $30, $14
sw $5, -4($30)
sub $30, $30, $14
sw $6, -4($30)
sub $30, $30, $14
sw $7, -4($30)
sub $30, $30, $14

; call merge function
jalr $19

; get $4, $5, $6, $7 back
add $30, $30, $14
lw $7, -4($30)
add $30, $30, $14
lw $6, -4($30)
add $30, $30, $14
lw $5, -4($30)
add $30, $30, $14
lw $4, -4($30)

; update $1 and $2
add $2, $0, $0
add $2, $2, $6
add $1, $0, $0
add $1, $1, $7

; recursive calling split
jalr $15

; save $4, $5, $6, $7 to tack
sw $4, -4($30)
sub $30, $30, $14
sw $5, -4($30)
sub $30, $30, $14
sw $6, -4($30)
sub $30, $30, $14
sw $7, -4($30)
sub $30, $30, $14

; call merge function
jalr $19

; get $4, $5, $6, $7 back
add $30, $30, $14
lw $7, -4($30)
add $30, $30, $14
lw $6, -4($30)
add $30, $30, $14
lw $5, -4($30)
add $30, $30, $14
lw $4, -4($30)

; restore $1, $2, $31
add $30, $30, $14
lw $31, -4($30)
add $30, $30, $14
lw $2, -4($30)
add $30, $30, $14
lw $1, -4($30)

; return to the main program
jr $31


merge:
; the list is merged
bne $4, $0, 2
bne $6, $0, 1
jr $31

; $23 is for slt
; get the first of the array from both arrays
lw $21, 0($5)
lw $22, 0($7)
slt $23, $21, $22
bne $23, $0, con

sub $24, $7, $5
divu $24, $14
mflo $24
add $25, $0, $7
loop:
lw $26, -4($7)
sw $26, 0($7)
sub $7, $7, $14
sub $24, $24, $17
bne $24, $0, loop 
sw $22, 0($5)
sub $6, $6, $17
add $7, $0, $0
add $7, $7, $25
add $7, $7, $14
jr $19

con:
sub $4, $4, $17
add $5, $5, $14
jr $19





