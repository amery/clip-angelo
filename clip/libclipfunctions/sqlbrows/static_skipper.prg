STATIC FUNCTION Skipper(n,browse)
	LOCAL i:=0
	LOCAL r := browse:rowset

	IF n > 0
		WHILE i < n
			r:Skip(1)
			IF r:Eof()
				EXIT
			ENDIF
			i++
		ENDDO
	ELSEIF n < 0
		WHILE i > n
			r:Skip(-1)
			IF r:Bof()
				EXIT
			ENDIF
			i--
		ENDDO
	ENDIF
RETURN i
