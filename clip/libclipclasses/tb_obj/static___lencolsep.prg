static func __lenColSep(num)
	local colSep
	colSep:=::__columns[num]:colSep
	colsep:=iif(colSep==NIL,::colSep,colSep)
return	len(colSep)
