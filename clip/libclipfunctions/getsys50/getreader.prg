proc GetReader( get )
	if ( GetPreValidate(get) )
		get:SetFocus()
		while ( get:exitState == GE_NOEXIT )
			if ( get:typeOut )
				get:exitState := GE_ENTER
			end
			while ( get:exitState == GE_NOEXIT )
				GetApplyKey( get, Inkey(0) )
			end
			if ( !GetPostValidate(get) )
				get:exitState := GE_NOEXIT
			end
		end
		get:KillFocus()
	end
return
