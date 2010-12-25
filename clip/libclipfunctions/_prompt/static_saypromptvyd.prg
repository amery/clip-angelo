static function sayPromptVyd(n,colVyb,colOsn)
	local row:=set(_SET_MESSAGE)
	local _colOsn:=set("MENUTO_MESSAGE_COLOR")
	if empty(_colOsn)
		_colOsn := colOsn
	endif
	dispOutAt( massPrompt[n][1],massPrompt[n][2], massPrompt[n][3], colVyb )
	if !empty(massprompt[n][4]) .and. row>0
		if valtype(massprompt[n][4]) =="B"
			eval(massprompt[n][4])
		else
			if set(_SET_MCENTER)
				dispOutAt( row,0, padc(massprompt[n][4],maxcol()), _colOsn )
			else
				dispOutAt( row,0, massprompt[n][4], _colOsn )
			endif
		endif
	endif
	//setpos(massPrompt[n][1],massPrompt[n][2])
	dispOutAt(massPrompt[n][1],massPrompt[n][2],"")
return
