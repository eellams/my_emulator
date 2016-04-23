; Note that spaces ARE REQUIRED
; Interestingly, commas are not, but use them if you like them!

; Adds all equal or less than memory 1 to r1, then finishes

; Assumes everything except BZ and JMPI work

> 0, 0 ; Whatever is in here will be overwritten - has to be included though
> 1, 10 ; How many times to repeat the loop

ADDI 1, 3   ; R3 += 1
LOAD 2, 3   ; R2 := M[R1] ; Sets R2 to contents of address 1
ADD 1, 2    ; R1 := R1 + R2
ADDI -1, 2  ; R2 -= 1
BZ 2        ; If zero flag is set, jump to FINISH
JMPI -3     ; Repeat until 0

; Inspect .csv for results
;  should be clear that R1 is being incremented

FINISH
