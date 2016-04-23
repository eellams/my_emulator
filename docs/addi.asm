; Note that spaces ARE REQUIRED
; Interestingly, commas are not, but use them if you like them - I do!

; Assumes that the following commands work:
;  JMPI (to jump from first byte of memory)

> 0, 0        ; Required

ADDI 1, 3     ; R3 := R3 + 1  [1]
ADDI 2, 3     ; R3 := R3 + 2  [3]
ADDI -3, 3    ;               [0]
FINISH
