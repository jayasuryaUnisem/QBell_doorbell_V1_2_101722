#DEFINE		RF_SYNC_BCHcoding		0

IF RF_SYNC_BCHcoding
;============================================================================
INCLUDE		MCU.inc
INCLUDE		MACRO.inc
INCLUDE		BCH32.inc

;============================================================================
BCH_DATA		.SECTION	'DATA'
_mLap			DB	4 dup(?)
mAA				DB	4 dup(?)
mGG				DB	4 dup(?)
mTmp			DB	4 dup(?)
mLpCnt			DB	?
mLpCntTmp		DB	?
mMask			DB	?


;========================================================
EXTERN	_mRFSync	:BYTE		;4-byte
PUBLIC	_mLap				;4-byte

;--------------------------------------------------------
PUBLIC	_BCH32_Syncword		;stack = 1, Input : _mLap[3:0], Output : mRFSync[3:0]	


;============================================================================
BCH_CODE		.SECTION	PARA	'CODE'

;--------------------------------------------------------
RvsBitValue		PROC
				ADDM	A,PCL
				RET		A,80h
				RET		A,40h
				RET		A,20h
				RET		A,10h
				RET		A,08h
				RET		A,04h
				RET		A,02h
				RET		A,01h
RvsBitValue		ENDP

;--------------------------------------------------------
ShiftRightGG	PROC
				CLR		C
				RRC		mGG[3]
				RRC		mGG[2]
				RRC		mGG[1]
				RRC		mGG[0]
				RET
ShiftRightGG	ENDP

;--------------------------------------------------------
ShiftLeftAA		PROC
				m_MOV	mLpCntTmp,BCH_32_N-BCH_32_K
	sla_lp:				
				CLR		C
				RLC		mAA[0]
				RLC		mAA[1]
				RLC		mAA[2]
				RLC		mAA[3]
				SDZ		mLpCntTmp
				JMP		sla_lp
				RET
ShiftLeftAA		ENDP

;--------------------------------------------------------
;Input  : _mLap[3:0]
;Output : mRFSync[3:0]
;Stack = 1
;--------------------------------------------------------
_BCH32_Syncword	PROC
				m_AND	_mLap[0],LOW LAP_MASK
				m_AND	_mLap[1],LOW (LAP_MASK>>8)
				m_AND	_mLap[2],LOW (LAP_MASK>>16)
				m_AND	_mLap[3],LOW (LAP_MASK>>24)
				
				MOV		A,LAP_BIT_MASK
				AND		A,_mLap[0]
				SNZ		Z
				JMP		bs_1
				MOV		A,LOW (LAP_BIT_MASK>>8)
				AND		A,_mLap[1]
				SNZ		Z
				JMP		bs_1
				MOV		A,LOW (LAP_BIT_MASK>>16)
				AND		A,_mLap[2]
				SNZ		Z
				JMP		bs_1
				MOV		A,LOW (LAP_BIT_MASK>>24)
				AND		A,_mLap[2]
				SZ		Z
				JMP		bs_0
				
	bs_1:
				m_OR	_mLap[0],LOW BARKER_PAD1
				m_OR	_mLap[1],LOW (BARKER_PAD1>>8)
				m_OR	_mLap[2],LOW (BARKER_PAD1>>16)
				m_OR	_mLap[3],LOW (BARKER_PAD1>>24)
				JMP		bs_01				

	bs_0:
				m_OR	_mLap[0],LOW (BARKER_PAD0)
				m_OR	_mLap[1],LOW (BARKER_PAD0>>8)
				m_OR	_mLap[2],LOW (BARKER_PAD0>>16)
				m_OR	_mLap[3],LOW (BARKER_PAD0>>24)
	
	bs_01:
	
				m_MOV	mAA[0],_mLap[0]
				m_MOV	mAA[1],_mLap[1]
				m_MOV	mAA[2],_mLap[2]
				m_MOV	mAA[3],_mLap[3]
				
				CALL	ShiftLeftAA
				
				m_MOV	mGG[0],G_32_L0
				m_MOV	mGG[1],G_32_L1
				m_MOV	mGG[2],G_32_L2
				m_MOV	mGG[3],G_32_L3
				
				MOV		A,P_32_L0
				XOR		A,mAA[0]
				AND		A,0C0h
				MOV		mTmp[0],A
				
				MOV		A,P_32_L1
				XOR		A,mAA[1]
				MOV		mTmp[1],A
				
				MOV		A,P_32_L2
				XOR		A,mAA[2]
				MOV		mTmp[2],A
				
				MOV		A,P_32_L3
				XOR		A,mAA[3]
				MOV		mTmp[3],A
				
				CLR		mLpCnt
	bs_Lp:
				MOV		A,07h
				AND		A,mLpCnt
				CALL	RvsBitValue			;stack=1+0
				MOV		mMask,A
				
				RRA		mLpCnt
				RR		ACC
				RR		ACC
				AND		A,1Fh
				CPL		ACC
				ADD		A,(OFFSET mTmp[0])+4
				MOV		MP0,A
				MOV		A,IAR0
				AND		A,mMask
				SZ		Z
				JMP		$01
				m_XOR	mTmp[0],mGG[0]
				m_XOR	mTmp[1],mGG[1]
				m_XOR	mTmp[2],mGG[2]
				m_XOR	mTmp[3],mGG[3]
				
		$01:
				CALL	ShiftRightGG		;stack=1+0
				INC		mLpCnt
				MOV		A,mLpCnt
				XOR		A,BCH_32_K
				SNZ		Z
				JMP		bs_Lp				
				
				m_XOR	mTmp[0],P_32_L0
				m_AND	mTmp[0],03Fh
				m_AND	mAA[0],0C0h
				m_OR	mTmp[0],mAA[0]	
				
				m_MOV	mTmp[1],mAA[1]
				m_MOV	mTmp[2],mAA[2]
				m_MOV	mTmp[3],mAA[3]
					
				m_MOV	_mRFSync[0],mTmp[3]
				m_MOV	_mRFSync[1],mTmp[2]
				m_MOV	_mRFSync[2],mTmp[1]
				m_MOV	_mRFSync[3],mTmp[0]
				RET
_BCH32_Syncword	ENDP

ENDIF
