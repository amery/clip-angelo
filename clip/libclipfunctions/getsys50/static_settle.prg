static func Settle(GetList, pos)
	local exitState
	status:LastPos := pos
	if ( pos == 0 )
		exitState := GE_DOWN
	else
		exitState := GetList[pos]:exitState
	end
	if ( exitState == GE_ESCAPE .or. exitState == GE_WRITE )
	    return ( 0 )
	end
	if ( exitState <> GE_WHEN )
		status:LastPos := pos
		status:BumpTop := .f.
		status:BumpBot := .f.

	else
		exitState := status:LastExit
	end
	do case
	case ( exitState == GE_UP )
		pos --
	case ( exitState == GE_DOWN )
		pos ++
	case ( exitState == GE_TOP )
		pos := 1
		status:BumpTop := .T.
		exitState := GE_DOWN
	case ( exitState == GE_BOTTOM )
		pos := Len(GetList)
		status:BumpBot := .T.
		exitState := GE_UP
	case ( exitState == GE_ENTER )
		pos ++
	endcase

	if ( pos == 0 )             // О©╫О©╫О©╫О©╫О©╫О©╫О©╫ О©╫О©╫О©╫О©╫О©╫ О©╫О©╫О©╫О©╫О©╫
		if ( !ReadExit() .and. !status:BumpBot )
			status:BumpTop := .T.
			pos := status:LastPos
			exitState := GE_DOWN
		end
	elseif ( pos == Len(GetList) + 1 )    // О©╫О©╫О©╫О©╫О©╫О©╫О©╫ О©╫О©╫О©╫О©╫О©╫ О©╫О©╫О©╫О©╫
		if ( !ReadExit() .and. exitState <> GE_ENTER .and. !status:BumpTop )
			status:BumpBot := .T.
			pos := status:LastPos
			exitState := GE_UP
		else
			pos := 0
		end
	end
	status:LastExit := exitState
	if ( pos <> 0 )
		GetList[pos]:exitState := exitState
	end
return (pos)
