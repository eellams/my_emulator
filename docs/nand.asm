; Note that spaces ARE REQUIRED
; Interestingly, commas are not, but use them if you like them - I do!

; Assumes that the following commands work:
;  JMPI (to jump from first byte of memory)
;  ADDI
;  STORE

> 0, 0        ; Required
> 1, 1        ; Numbers to test
> 2, 0
> 3, 134
> 4, 0

ADDI 1, 3     ; R3 := R3 + 1
LOAD 1, 3     ; R1 := M[R3]
NAND 1, 0     ; R1 := ~(R1 & 0)  (effectively a NOT)
ADDI 1, 3
STORE 1, 3

ADDI 1, 3     ; R3 := R3 + 1
LOAD 1, 3     ; R1 := M[R3]
LOAD 2, 3
ADDI -2, 2    ; R2 := R2 - 2
NAND 1, 2     ; R1 := ~(R1 & R2)  (proper NOT test)

; Store in next memory address
ADDI 1, 3
STORE 1, 3


; Should be able to inspect log files to see items being written to / read from memory

FINISH

