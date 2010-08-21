function __CGET__(block,aSubscript,cName,cRealName,cPic,bValid,bWhen)
	local oget,row:=row(),col:=col()
	if empty(cName)
		cName := upper(cRealName)
	endif
	oGet := getnew(row(),col(),block,cName,cPic,,,bValid,bWhen)
	oGet:subscript := aSubscript
	oGet:realName  := cRealName
	setpos(row,col)
return oGet
