; Note that spaces ARE REQUIRED
; Interestingly, commas are not, but use them if you like them - I do!

; Sums all the numbers equal and bellow memory 1
; Stores in memory 2

> 0, 0      ; Whatever is in here will be overwritten - has to be included though
> 1, 10     ; How many times to repeat the loop
> 2, 0      ; Storage location
> 10, 0

ADDI 1, 3   ; R3 += 1
LOAD 2, 3   ; R2 := M[R1] ; Sets R2 to contents of address 1
ADD 1, 2    ; R1 := R1 + R2
ADDI -1, 2  ; R2 -= 1
BZ 2        ; If zero flag is set, jump to FINISH
JMPI -3     ; Repeat until 0


ADDI 1, 3   ; Increment to 2
STORE 1, 3  ; Store Reg1 in M[Reg3]
FINISH
