.import print
.import init
.import new
.import delete
lis $4
.word 4
sw $2, -4($30)
sub $30, $30, $4
sw $31, -4($30)
sub $30, $30, $4
lis $2
.word 0
lis $11
.word init
jalr $11
add $30, $30, $4
lw $31, -4($30)
add $30, $30, $4
lw $2, -4($30)
lis $11
.word 1
lis $3
.word 44
sub $30, $30, $3
add $29, $30, $0
add $3, $0, $0
sw $31, -4($30)
sub $30, $30, $4
sw $1, 36($29)
sw $2, 40($29)
lis $5
.word fmain
jr $5
fprintArray:
add $30, $30, $4
lw $8, -4($30)
sw $8, 4($29)
add $30, $30, $4
lw $8, -4($30)
sw $8, 8($29)
lis $5
.word 0
sw $5, 12($29)
whileloop0:
lw $3, 12($29)
sw $3, -4($30)
sub $30, $30, $4
lw $3, 8($29)
add $30, $30, $4
lw $5, -4($30)
slt $3, $5, $3
beq $3, $0, endwhileloop0
lw $3, 4($29)
sw $3, -4($30)
sub $30, $30, $4
lw $3, 12($29)
add $30, $30, $4
lw $5, -4($30)
mult $3, $4
mflo $3
add $3, $5, $3
lw $3, 0($3)
sw $1, -4($30)
sub $30, $30, $4
sw $31, -4($30)
sub $30, $30, $4
add $1, $3, $0
lis $5
.word print
jalr $5
add $30, $30, $4
lw $31, -4($30)
add $30, $30, $4
lw $1, -4($30)
lis $5
.word 12
add $3, $5, $29
sw $3, -4($30)
sub $30, $30, $4
lw $3, 12($29)
sw $3, -4($30)
sub $30, $30, $4
lis $3
.word 1
add $30, $30, $4
lw $5, -4($30)
add $3, $3, $5
add $30, $30, $4
lw $5, -4($30)
sw $3, 0($5)
beq $0, $0, whileloop0
endwhileloop0:
lis $3
.word 0
jr $31
ffoo:
add $30, $30, $4
lw $8, -4($30)
sw $8, 20($29)
lis $5
.word 1
sw $5, 28($29)
lis $5
.word 0
sw $5, 24($29)
lw $3, 20($29)
sw $3, -4($30)
sub $30, $30, $4
lis $3
.word 0
add $30, $30, $4
lw $5, -4($30)
slt $3, $3, $5
beq $3, $0, else0
lis $5
.word 28
add $3, $5, $29
sw $3, -4($30)
sub $30, $30, $4
lis $3
.word 10
sw $1, -4($30)
sub $30, $30, $4
sw $31, -4($30)
sub $30, $30, $4
add $1, $3, $0
lis $5
.word new
jalr $5
add $30, $30, $4
lw $31, -4($30)
add $30, $30, $4
lw $1, -4($30)
bne $3, $0, 1 
add $3, $11, $0
add $30, $30, $4
lw $5, -4($30)
sw $3, 0($5)
whileloop1:
lw $3, 24($29)
sw $3, -4($30)
sub $30, $30, $4
lis $3
.word 10
add $30, $30, $4
lw $5, -4($30)
slt $3, $5, $3
beq $3, $0, endwhileloop1
lw $3, 28($29)
sw $3, -4($30)
sub $30, $30, $4
lw $3, 24($29)
add $30, $30, $4
lw $5, -4($30)
mult $3, $4
mflo $3
add $3, $5, $3
sw $3, -4($30)
sub $30, $30, $4
lw $3, 20($29)
sw $3, -4($30)
sub $30, $30, $4
lw $3, 24($29)
add $30, $30, $4
lw $5, -4($30)
mult $3, $5
mflo $3
add $30, $30, $4
lw $5, -4($30)
sw $3, 0($5)
lis $5
.word 24
add $3, $5, $29
sw $3, -4($30)
sub $30, $30, $4
lw $3, 24($29)
sw $3, -4($30)
sub $30, $30, $4
lis $3
.word 1
add $30, $30, $4
lw $5, -4($30)
add $3, $3, $5
add $30, $30, $4
lw $5, -4($30)
sw $3, 0($5)
beq $0, $0, whileloop1
endwhileloop1:
lis $5
.word 24
add $3, $5, $29
sw $3, -4($30)
sub $30, $30, $4
sw $31, -4($30)
sub $30, $30, $4
lw $3, 20($29)
sw $3, -4($30)
sub $30, $30, $4
lis $3
.word 1
add $30, $30, $4
lw $5, -4($30)
sub $3, $5, $3
sw $3, -4($30)
sub $30, $30, $4
lis $5
.word ffoo
jalr $5
add $30, $30, $4
lw $31, -4($30)
add $30, $30, $4
lw $5, -4($30)
sw $3, 0($5)
lis $5
.word 24
add $3, $5, $29
sw $3, -4($30)
sub $30, $30, $4
sw $31, -4($30)
sub $30, $30, $4
lis $3
.word 10
sw $3, -4($30)
sub $30, $30, $4
lw $3, 28($29)
sw $3, -4($30)
sub $30, $30, $4
lis $5
.word fprintArray
jalr $5
add $30, $30, $4
lw $31, -4($30)
add $30, $30, $4
lw $5, -4($30)
sw $3, 0($5)
lw $3, 28($29)
sw $1, -4($30)
sub $30, $30, $4
sw $31, -4($30)
sub $30, $30, $4
add $1, $3, $0
div $1, $4
mfhi $5
bne $5, $0, 3
lis $5
.word delete
jalr $5
add $30, $30, $4
lw $31, -4($30)
add $30, $30, $4
lw $1, -4($30)
beq $0, $0, done0
else0:
done0:
lis $3
.word 0
jr $31
fmain:
sw $31, -4($30)
sub $30, $30, $4
lis $3
.word 10
sw $3, -4($30)
sub $30, $30, $4
lis $5
.word ffoo
jalr $5
add $30, $30, $4
lw $31, -4($30)
add $30, $30, $4
lw $31, -4($30)
jr $31
