function __DBUPDATE( cAlias, bKey, lRandom, bWith )
	local nSel, xKey, oErr, lErr := .F.

	iif(lRandom==NIL, lRandom:= .F.,)
	goto top
	nSel := Select()
	begin sequence
		select (cAlias)
		goto top
		do while ( !EOF() )
			xKey := eval(bKey)
			select (nSel)
			if ( lRandom )
				dbSeek(xKey, iif( .F., .T., Nil ))
				if ( Found() )
					eval(bWith)
				endif
			else
				do while ( eval(bKey) < xKey .AND. ! EOF() )
					skip
				enddo
				if ( eval(bKey) == xKey .AND. ! EOF() )
					eval(bWith)
				endif
			endif
			select (cAlias)
			skip
		enddo
	recover using oErr
		lErr := .T.
	end sequence
	select (nSel)
	if ( lErr )
		break( oErr )
	endif
return .T.
