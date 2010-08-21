STATIC FUNCTION Skipper(n, browse)
    LOCAL lAppend:=.f.
    LOCAL i:=0
    lAppend := IS_APPEND_MODE(browse)           // see #defines at top
    IF n == 0 .OR. RECCOUNT() == 0
	SKIP 0
    ELSEIF n > 0 .and. RECNO() != RECCOUNT() + 1
	DO WHILE i < n
	    SKIP 1
	    IF ( EOF() )
		IF ( lAppend )
		    i++
		ELSE
		    SKIP -1
		ENDIF
		EXIT
	    ENDIF
	    i++
	ENDDO
    ELSEIF n < 0
	DO WHILE i > n
	    SKIP -1
	    IF ( BOF() )
		EXIT
	    ENDIF
	    i--
	ENDDO
    ENDIF
RETURN i
