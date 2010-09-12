FUNCTION NUMNO(str,no)
	LOCAL I,J
	LOCAL nos := 0
	LOCAL pos
	LOCAL len := LEN(str)+1
	LOCAL digits := "1234567890.-"

	str := " "+str
	IF no>0
		FOR I:=1 TO len
			IF !(SUBSTR(str,I-1,1) $ digits) .AND.;
				SUBSTR(str,I,1) $ digits
				IF ++nos == no
					EXIT
				ENDIF
			ENDIF
		NEXT
	ELSE
		FOR I:=len TO 1 STEP -1
			IF !(SUBSTR(str,I-1,1) $ digits) .AND.;
				SUBSTR(str,I,1) $ digits
				IF ++nos == -no
					EXIT
				ENDIF
			ENDIF
		NEXT
	ENDIF
	J := I
	IF nos==ABS(no)
		WHILE J<=len .AND. SUBSTR(str,J,1) $ digits; J++; ENDDO
		RETURN SUBSTR(str,I,J-I)
	ENDIF
RETURN
