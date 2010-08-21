static function sayPrompt(n,colosn)
	local colDop, colDop1
	colDop:= colosn
	colDop:=substr(colDop,at('/',colDop))
	colDop1:=substr(colDop,at(',',colDop)+1)
	colDop1:=substr(colDop1,1,at('/',colDop1)-1)
	if colDop1==substr(colDop,2,at(',',colDop)-2)
		colDop:="14"+colDop
	else
		colDop:=colDop1+colDop
	endif
	dispOutAt(massPrompt[n][1],massPrompt[n][2], massPrompt[n][3], colosn)
	if masshotkey[n][1]>0
		dispOutAt(massPrompt[n][1],massPrompt[n][2]+massHotkey[n][1]-1, massHotkey[n][2], colDop)
	endif
return
