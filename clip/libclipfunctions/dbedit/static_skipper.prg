STATIC FUNCTION Skipper(n, browse)
    LOCAL i:=0
    IF n == 0 .OR. RECCOUNT() == 0
	SKIP 0
	return 0
    endif
    if n == 0
#ifdef DE_APPEND_SUPPORT
	 if Eof() .and. !m->__de_append_mode
		skip -1
		i := -1
	 else
		skip 0
	 endif
#else
	 skip 0
#endif
	 return i
    endif
    IF n > 0 .and. RECNO() != RECCOUNT() + 1
	DO WHILE i < n
	    SKIP 1
#ifdef DE_APPEND_SUPPORT
	    IF ( EOF() )
		IF ( m->__de_append_mode )
		    i++
		ELSE
		    SKIP -1
		ENDIF
		EXIT
	    ENDIF
#else
	    IF ( EOF() )
		SKIP -1
		EXIT
	    ENDIF
#endif
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
