; The program reads an array and place the maximum value in $3.
; Requires the array to not be empty
; $1 holds the address of the array
; $2 holds the number of element in the erray
; $5 is the result from comparing the maximum so far and the new element

; initialize the $3 to 0
add $3, $0, $0
; initialize the $14 to 4 for the array index in each loop;
lis $14
.word 4
; intitialize the $11 for decrement of the array size in each loop
lis $11
.word 1

loop:
; get the value from the array and store into $4
lw $4, 0($1)
; compare the new element with the maximum so far
slt $5, $4, $3
bne $5, $0, 2
; set $3 to 0
add $3, $0, $0
; add the value to $3
add $3, $3, $4
; increment the index
add $1, $1, $14
; decrease the array size for the next base case check
sub $2, $2, $11
; decide to loop or not
bne $2, $0, loop
; return
jr $31
