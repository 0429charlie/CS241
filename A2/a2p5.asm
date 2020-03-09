; The program reads an array and print out each corresponding character for each element
; $1 is the address of the array
; $2 is the number of element of the erray

; initialize the $3 to 0
add $3, $0, $0
; initialize the $14 to 4 for the loop index in each loop;
lis $14
.word 4
; intitialize the $11 for decrement of the array size in each loop
lis $11
.word 1
; initialize the $12 to be 64 that is used for conversion from the number to character
lis $12
.word 64
lis $15
.word 0xffff000c ; output
; space in 
lis $16
; space in ASCII
.word 32

loop:
; check the length of the array
beq $2, $0, done
; get the value from the array and store into $4
lw $4, 0($1)
; set $3 to 0
add $3, $0, $0
; check if the element is 0 or not
beq $4, $0, zero
; add the corresponding character to $3
add $3, $12, $4
; send the character to standard output
sw $3, 0($15)
; increment the index
add $1, $1, $14
; decrease the array size for the next base case check
sub $2, $2, $11
; to the begining of the loop
beq $0, $0, loop

done:
; return
jr $31

zero:
; set $3 to the character space
add $3, $3, $16
; send the character to standard output
sw $3, 0($15)
; increment the index
add $1, $1, $14
; decrease the array size for the next base case check
sub $2, $2, $11
; to the begining of the loop
beq $0, $0, loop

