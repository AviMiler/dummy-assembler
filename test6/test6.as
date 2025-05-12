; some comments

; some macros
mcro MATHOPS    
    mov @r4,  DATASEGMENT1
    add @r3, @r3
    sub @r0, @r7
endmcro

mcro REGOPS    
clr @r0
dec @r1
    jmp CHECKSEGMENT
    prn STRING1
endmcro

.extern SEGMENTONE
.extern EXTERNSEG1
.extern CHECKSEGMENT
.extern EXTERNSEG3
.extern EXTERNSEG4

PROGRAMSTART: mov @r1, STRINGSEG1
    red @r0
    MATHOPS
    jmp SEGMENTONE
    prn STRINGSEG2

DATASEGMENT1:     .data 1, 2, 3, 4, 5
DATASEGMENT2:     .data -1, -2, -3, -4, -5
STRINGSEG1:       .string "MegaAssemblerTestSegOne"
STRINGSEG2:       .string "MegaAssemblerTestSegTwo"
STRINGSEG3:       .string "MegaAssemblerTestSegThree"
STRINGSEG4:       .string "MegaAssemblerTestSegFour"
STRINGSEG5:       .string "MegaAssemblerTestSegFive"
VALSEGMENT1:      .data 111 
VALSEGMENT2:      .data 222
VALSEGMENT3:      .data 333
VALSEGMENT4:      .data 444
REGOPS 
.extern STRING1
.extern VALSEGMENT6

