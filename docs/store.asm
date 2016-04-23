; Note that spaces ARE REQUIRED
; Interestingly, commas are not, but use them if you like them - I do!

; Assumes that the following commands work:
;  JMPI (to jump from first byte of memory)
;  ADDI
;  STORE

> 0, 0        ; Required
> 1, 1        ; Numbers to test
> 2, 0
> 3, 2
> 4, 0
> 5, 200
> 6, 0

ADDI 1, 3     ; R3 := R3 + 1
LOAD 1, 3     ; R1 := M[R3]
ADDI 1, 3
STORE 1, 3


ADDI 1, 3     ; R3 := R3 + 1
LOAD 1, 3     ; R1 := M[R3]
ADDI 1, 3
STORE 1, 3

ADDI 1, 3     ; R3 := R3 + 1
LOAD 1, 3     ; R1 := M[R3]
ADDI 1, 3
STORE 1, 3

; Should be able to inspect log files to see items being nand'd

FINISH

