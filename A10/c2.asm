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
.word 28
sub $30, $30, $3
add $29, $30, $0
add $3, $0, $0
sw $31, -4($30)
sub $30, $30, $4
sw $1, 20($29)
sw $2, 24($29)
lis $5
.word fmain
jr $5
fthree:
add $30, $30, $4
lw $8, -4($30)
sw $8, 4($29)
add $30, $30, $4
lw $8, -4($30)
sw $8, 8($29)
add $30, $30, $4
lw $8, -4($30)
sw $8, 12($29)
lw $3, 4($29)
sw $3, -4($30)
sub $30, $30, $4
lw $3, 8($29)
add $30, $30, $4
lw $5, -4($30)
mult $3, $5
mflo $3
sw $3, -4($30)
sub $30, $30, $4
lw $3, 12($29)
add $30, $30, $4
lw $5, -4($30)
add $3, $3, $5
jr $31
fmain:
sw $31, -4($30)
sub $30, $30, $4
lis $3
.word 20
sw $3, -4($30)
sub $30, $30, $4
lw $3, 12($29)
sw $3, -4($30)
sub $30, $30, $4
lw $3, 8($29)
sw $3, -4($30)
sub $30, $30, $4
lis $5
.word fthree
jalr $5
add $30, $30, $4
lw $31, -4($30)
add $30, $30, $4
lw $31, -4($30)
jr $31
