; The program reads an array and place -1 into $3 if the array is empty or place the last element of the array into $3.
; $1 is the address of tehe array
; $2 is the number of element of the erray

; initialize the $3 to 0
add $3, $0, $0
; initialize the $14 to 4 for the loop index in each loop;
lis $14
.word 4
; intitialize the $11 for decrement of the array size in each loop
lis $11
.word 1

; if it is the empty array
beq $2, $0, empty 

loop:
; get the value from the array and store into $4
lw $4, 0($1)
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

empty:
; load -1 into $4
lis $4
.word -1
; set $3 to -1 using $4 
add $3, $3, $4
; return
jr $31
