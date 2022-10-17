;========================================================
m_MOV	MACRO	d1,d2
		MOV		A,d2
		MOV		d1,A
		ENDM

m_XOR	MACRO	d1,d2
		MOV		A,d2
		XORM	A,d1
		ENDM		

m_XORA	MACRO	d1,d2
		MOV		A,d2
		XOR		A,d1
		ENDM		

m_AND	MACRO	d1,d2
		MOV		A,d2
		ANDM	A,d1
		ENDM		

m_OR 	MACRO	d1,d2
		MOV		A,d2
		ORM		A,d1
		ENDM

m_DECW	MACRO	bl,bh
		MOV		A,bl
		SUB		A,01h
		MOV		bl,A
		SNZ		C
		DEC		bh
		ENDM
		
m_SKZW	MACRO	bl,bh
		MOV		A,bl
		OR		A,bh
		SNZ		Z
		ENDM
		