STATIC FUNCTION AtAny(sub,str)
	LOCAL I := 0
	LOCAL c

	DO WHILE !empty( c := SUBSTR(str,++I,1) )
		IF c $ sub
			EXIT
		ENDIF
	ENDDO
RETURN I
