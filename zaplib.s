;written by Doug Fraker 2019-2020
;NES zapper gun code


.export _zap_shoot, _zap_read

; FIXME wKaro
; see beginning of crt0.s
; CTRL_PORT1	=$4016
; CTRL_PORT2	=$4017
;	.segment "ZEROPAGE"

;	NTSC_MODE: 			.res 1
;	FRAME_CNT1: 		.res 1
;	FRAME_CNT2: 		.res 1
;	VRAM_UPDATE: 		.res 1
;       ...
; and neslib.s
;	NMI handler
;        nmi:
;        ...
;        @skipAll:
;
;                lda <PPU_MASK_VAR
;                sta PPU_MASK
;
;                inc <FRAME_CNT1
;                inc <FRAME_CNT2
;                lda <FRAME_CNT2
;                cmp #6
;                bne @skipNtsc
;                lda #0
;                sta <FRAME_CNT2
;        ...

CTRL_PORT1 =$4016
;FRAME_CNT1 =$01

; or memory browser:
; 0000  68                                               FT_TEMP
; 0001     9D                                            __STARTUP__
; 0002        00                                         NES_PRG_BANKS
; 0003           80                                      NTSC_MODE
; 0004              91                                   FRAME_CNT1
; 0005                 01                                FRAME_CNT2

FRAME_CNT1 =$04

; \FIXME

;from NESDEV WIKI
;7  bit  0
;---- ----
;xxxT WxxS
;   | |  |
;   | |  +- Serial data (Vs.)
;   | +---- Light sense (0: detected; 1: not detected) (NES/FC)
;   +------ Trigger (0: released; 1: pulled) (NES/FC)


;assumed that dpmc errors won't happen
;usually zappers are in controller port 2
;but this can do either port
;in value = 0 or 1, which port
;out value = 0 or 1, 1 if zapper trigger pulled

_zap_shoot:
	tay
	ldx #1
	stx CTRL_PORT1
	dex ;#0
	stx CTRL_PORT1
	
	lda CTRL_PORT1,y
	and #$10
	beq @exit
	ldx #1 ;x is 0 otherwise
	
@exit:
	txa
	ldx #0
	rts
	
	
	
;in value = 0 or 1, which port
;out value = 0 or 1, whether it detected light
;please, only display 1 white box per frame
	
_zap_read:	
	tay
	ldx <FRAME_CNT1 ;ticks up during nmi code
	
@loop:
	lda #1
	sta CTRL_PORT1
	lda #0
	sta CTRL_PORT1
	lda CTRL_PORT1,y
	and #$08
	beq @hit_yes ;0 = light, 8 = no light
	cpx <FRAME_CNT1
	beq @loop 
	
@hit_no: ;ran past the frame and into the next nmi
	lda #0
	tax
	rts
	
@hit_yes:
	lda #1
	ldx #0
	rts
		
	