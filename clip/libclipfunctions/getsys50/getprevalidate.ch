func GetPreValidate(get)
	local saveUpdated
	local when := .t.
	if ( get:preBlock <> NIL )
		saveUpdated := status:Updated
		when := Eval(get:preBlock, get)
		get:Display()
		ShowScoreBoard()
		status:Updated := saveUpdated
	end
	if ( status:KillRead )
		when := .f.
		get:exitState := GE_ESCAPE
	elseif ( !when )
		get:exitState := GE_WHEN
	else
		get:exitState := GE_NOEXIT
	end
return (when)
