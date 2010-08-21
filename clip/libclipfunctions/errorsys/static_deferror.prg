static func DefError(e)
local i, cMessage, aOptions, nChoice

	while dispcount()>0
	   dispend()
	enddo
	set(20,"SCREEN")

	if "ARGS" $ e
		for i=1 to len(e:args)
			if valtype(e:args[i]) == "O"
				e:args[i] := "{OBJECT}"
			endif
		next
	endif

	outlog("object error:",e)
	outlog("object mapkeys:",mapkeys(e))

	cMessage := ErrorMessage(e)

	if ( e:genCode == EG_ZERODIV )
		return (0)
	end

	if ( e:genCode == EG_SIGNAL )
		if isFunction("CLIP_SHUTDOWN")
			clip("CLIP_SHUTDOWN",e)
		endif
		quit
	endif

	aOptions := { EG_MSG_QUIT }

	if (e:canRetry)
		AAdd(aOptions, EG_MSG_RETRY)
	end

	if (e:canDefault)
		AAdd(aOptions, EG_MSG_DEFAULT)
	end

	nChoice := 0
	while ( nChoice == 0 )

		if ( Empty(e:osCode) )
			if IsFunction("gtk_InitCheck") .and. clip("gtk_InitCheck")
				nChoice := clip("gtk_ErrorMsgBox", cMessage, aOptions, [Error] )
			else
				nChoice := Alert( cMessage, aOptions )
			endif

		else
			if IsFunction("gtk_InitCheck") .and. clip("gtk_InitCheck")
				nChoice := clip("gtk_ErrorMsgBox", cMessage + ;
							";(OS Error " + NTRIM(e:osCode) + ")", ;
							aOptions,[Error] )
			else
				nChoice := Alert( cMessage + ;
							";(OS Error " + NTRIM(e:osCode) + ")", ;
							aOptions )
			endif
		end


		if ( nChoice == NIL )
			exit
		end

	end

	if ( !Empty(nChoice) )

		// do as instructed
		if ( aOptions[nChoice] == EG_MSG_QUIT )
			Break(e)

		elseif ( aOptions[nChoice] == EG_MSG_RETRY )
			return (.t.)

		elseif ( aOptions[nChoice] == EG_MSG_DEFAULT )
			return (.f.)

		end

	end

	ErrorLevel(1)
	QUIT

return (.f.)
