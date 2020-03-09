; The program reads an integer array as input and print out each element
; $1 holds the address of the array
; $2 holds the number of element in the erray

; initialize the $14 to 4 for the array index in each loop;
lis $14
.word 4
; intitialize the $11 for decrement of the array size in each loop
lis $11
.word 1

beq $2, $0, done

loop:
; copy the important register before the procedure call
add $21, $0, $0
add $21, $21, $1
add $22, $0, $0
add $22, $22, $2
add $24, $0, $0
add $24, $24, $14
add $23, $0, $0
add $23, $23, $11


; get the value from the array and store into $4
lw $4, 0($1)
; save $4 into $1
add $1, $0, $0
add $1, $1, $4
lis $5
.word print
; call procedure
jalr $5

; sign the value back after the procedure call
add $1, $0, $0
add $1, $1, $21
add $2, $0, $0
add $2, $2, $22
add $14, $0, $0
add $14, $14, $24
add $11, $0, $0
add $11, $11, $23

; increment the index
add $1, $1, $14
; decrease the array size for the next base case check
sub $2, $2, $11
; decide to loop or not
bne $2, $0, loop

done:
; return
jr $31

