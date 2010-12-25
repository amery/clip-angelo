FUNCTION dbEvalOptimize( b, cFor, xWhile, nNext, nRec, lRest)
	LOCAL __bKey, __bWhile, OldSel
	LOCAL h,o,f

	IF !USED()
		RETURN
	ENDIF

	OldSel:=SELECT()

	IF ( valtype( xWhile )=="C" )
		__bWhile:="{||"+xWhile+"}"
		__bWhile:=&__bWhile
	ELSE
		IF ( valtype( xWhile )!="B" )
			__bWhile:={ ||.t. }
		ELSE
			__bWhile:=xWhile
			lRest:= .T.
		ENDIF
	ENDIF

	IF (f := ISFILTERED()) .AND. !EMPTY(cFor)
		o := m6_FiltCopy(m6_GetAreaFilter())
		h := m6_NewFilter(cFor)
		m6_FiltJoin(m6_GetAreaFilter(),h,2)
	ELSEIF !EMPTY(cFor)
		m6_SetAreaFilter(m6_NewFilter(cFor))
	ENDIF

	lRest:=Iif( lRest==NIL, .F., lRest )

	IF ( nRec != NIL )
		GOTO nRec
		nNext:= 1
	ELSE
		IF ( nNext==NIL )
			nNext:= -1
		ELSE
			lRest:= .T.
		ENDIF
		IF ( !lRest )
			GOTO top
		ENDIF
	ENDIF

	WHILE ( !EOF() .AND. nNext-- != 0 .AND. eval( __bWhile ) )
		EVAL(b)
		IF nNext != 0
			SELECT (OldSel)
			SKIP
		ENDIF
	ENDDO

	SELECT (OldSel)

	IF !EMPTY(cfor)
		DBCLEARFILTER()
		IF f
			m6_SetAreaFilter(o)
		ENDIF
	ENDIF
	SELECT(oldsel)
RETURN
