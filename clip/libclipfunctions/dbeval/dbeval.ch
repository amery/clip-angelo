FUNCTION dbEval( b, xFor, xWhile, nNext, nRec, lRest)
	LOCAL __bKey, __bFor, __bWhile, OldSel
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

	IF ( valtype( xFor )=="C" )
		__bFor:="{||"+xFor+"}"
		__bFor:=&xFor
	ELSE
		IF ( valtype( xFor )!="B" )
			__bFor:={ ||.t. }
		ELSE
			__bFor:=xFor
		ENDIF
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
		IF ( eval( __bFor ) )
			EVAL(b)
		ENDIF
		IF nNext != 0
			SELECT (OldSel)
			SKIP
		ENDIF
	ENDDO

	SELECT (OldSel)
RETURN
