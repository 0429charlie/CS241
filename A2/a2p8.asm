; The program reads the tree as an array and figure out its height
; save return address to other register
add $4, $0, $0
add $4, $4, $31
; save the maximumheightsofar to $3 (set it to 1 initially)
lis $3
.word 1
; save the current height and initially set it to 1
lis $5
.word 1
; $1 is the initial address of the tree in array form
; save the current address to $6 and set it initially to initial address
add $6, $0, $0
add $6, $6, $1
; $9 is for sltu comparison
; $10 is always 1 to update the height
lis $10
.word 1
; $11 is always 4 to update the address
lis $11
.word 4
; $12 is always -1 to check the child
sub $12, $0, $10
; $13 is used to store the result of multu 
; $14 stores to id of the place to jump to
lis $14
.word findheight


findheight:
; Base Case:

; load the two children of the current node and compare their value
lw $7, 4($6)
lw $8, 8($6)
bne $8, $7, 5
sltu $9, $3, $5
beq $9, $0, 2
add $3, $0, $0
add $3, $3, $5
jr $31

; Recurssive Case:

; Left child
beq $7, $12, 22
; push the current height onto the stack and update the stack address
sw $5, -4($30)
sub $30, $30, $11
; push the current address onto the stack and update the stack address
sw $6, -4($30)
sub $30, $30, $11
; update the currenthight
add $5, $5, $10 
; update the current address
add $6, $0, $0
multu $7, $11
mflo $13
add $6, $1, $13

; push the value of children onto the stack
sw $7, -4($30)
sub $30, $30, $11
sw $8, -4($30)
sub $30, $30, $11

jalr $14

; pull the value of children back
add $30, $30, $11
lw $8, -4($30)
add $30, $30, $11
lw $7, -4($30)

; pull the current address back
add $30, $30, $11 
lw $6, -4($30)
; pull the current height back
add $30, $30, $11
lw $5, -4($30)


; Right child
beq $8, $12, 22
; push the current height onto the stack and update the stack address
sw $5, -4($30)
sub $30, $30, $11
; push the current address onto the stack and update the stack address
sw $6, -4($30)
sub $30, $30, $11
; update the currenthight
add $5, $5, $10
; update the current address
add $6, $0, $0
multu $8, $11
mflo $13
add $6, $1, $13

; push the value of children onto the stack
sw $7, -4($30)
sub $30, $30, $11
sw $8, -4($30)
sub $30, $30, $11

jalr $14

; pull the value of children back
add $30, $30, $11
lw $8, -4($30)
add $30, $30, $11
lw $7, -4($30)

; pull the current address back
add $30, $30, $11
lw $6, -4($30)
; pull the current height back
add $30, $30, $11
lw $5, -4($30)


; return
add $31, $0, $0
add $31, $31, $4
jr $31
