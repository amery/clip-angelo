function __AtPrompt(row,col,prompt,msg)
	local pos, colDop, colDop1 ,i
	colDop:= setcolor()
	colDop:=substr(colDop,at('/',colDop))
	colDop1:=substr(colDop,at(',',colDop)+1)
	colDop1:=substr(colDop1,1,at('/',colDop1)-1)
	if colDop1==substr(colDop,2,at(',',colDop)-2)
		colDop:="14"+colDop
	else
		colDop:=colDop1+colDop
	endif
	pos:=at('~',prompt)
	if pos>0
		prompt:=substr(prompt,1,pos-1)+substr(prompt,pos+1)
		aadd(massHotkey,{pos,lower(substr(prompt,pos,1))})
	else
		//aadd(massHotkey,{0,substr(HOT_KEYS,nhotkey,1)})
		aadd(massHotkey,{0,lower(left(alltrim(prompt),1))})
		nhotkey++
	endif
	aadd(massPrompt,{int(row),int(col),prompt,msg})
	dispOutAt(row, col, prompt )
	i=len(masshotkey)
	if pos>0
		dispOutAt( row, col+pos-1, massHotkey[len(massHotkey)][2], colDop)
	endif
return .f.
