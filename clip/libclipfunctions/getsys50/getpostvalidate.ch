func GetPostValidate(get)
	local saveUpdated
	local changed, valid := .t.
	if ( get:exitState == GE_ESCAPE )
	    return (.t.)
	end
	get:Reset()
	if ( get:BadDate() )
		get:Home()
		DateMsg()
		ShowScoreboard()
		return (.f.)
	end
	if ( get:changed )
		get:Assign()
		status:Updated := .t.
	end
	get:Reset()
	if ( get:postBlock <> NIL )
		saveUpdated := status:Updated
		SetPos( get:row, get:col + Len(get:buffer) )
		valid := Eval(get:postBlock, get)
		SetPos( get:row, get:col )
		ShowScoreBoard()
		get:UpdateBuffer()
		status:Updated := saveUpdated
		if ( status:KillRead )
			get:exitState := GE_ESCAPE  // О©╫О©╫О©╫О©╫О©╫О©╫О©╫ О©╫О©╫О©╫О©╫О©╫ О©╫О©╫ ReadModal()
			valid := .t.
		end
	end
return (valid)
