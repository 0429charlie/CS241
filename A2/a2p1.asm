; The program  compares $1 and $2 and place the one with larger value in $3
; $4 is 1 if $1 < $2

; set the $3 to be 0
add $3, $0, $0
; compare the $1 and $2. Set $4 to be 1 if $1 < $2
slt $4, $1, $2
; decide whether write $1 into $3 or $2 into $3
beq $4, $0, 2
; if $2 > $1
add $3, $3, $2
;return
jr $31
; if $1 > $2
add $3, $3, $1
; return
jr $31
