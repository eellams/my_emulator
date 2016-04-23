; Note that spaces ARE REQUIRED
; Interestingly, commas are not, but use them if you like them - I do!

; Assumes that the following commands work:
;  JMPI (to jump from first byte of memory)
;  ADDI

> 0, 0        ; Required
> 1, 1        ; Numbers to add
> 2, 3
> 3, 200

ADDI 1, 3     ; R3 := R3 + 1
LOAD 1, 3     ; R1 := M[R3]

ADDI 1, 3     ; R3 := R3 + 1
LOAD 1, 3     ; R1 := M[R3]

ADDI 1, 3     ; R3 := R3 + 1
LOAD 1, 3     ; R1 := M[R3]

; Should be able to see R1 changing in .csv file

FINISH

