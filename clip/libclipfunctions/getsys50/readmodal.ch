func ReadModal( GetList )
	local get,ret , pos:=1, savedGetSysVars
	__getlist:=getList
	if status==NIL
	   __getSysInit()
	endif
	if ( ValType(status:Format) == "B" )
		Eval(status:Format)
	end
	if ( Empty(getList) )
	     SetPos( MaxRow()-1, 0 )
	     return (.f.)
	end
	savedGetSysVars := ClearGetSysVars()
	status:ReadProcName := ProcName(1)
	status:ReadProcLine := ProcLine(1)
	pos := Settle( Getlist, 0 )
	status:currentGet := pos

	while ( pos <> 0 )
		get := GetList[pos]
		PostActiveGet( get )
		if ( ValType( get:reader ) == "B" )
		      Eval( get:reader, get )
		else
		      GetReader( get )
		end
		pos := Settle( GetList, pos )
		status:currentGet=pos
	end
	ret:=status:updated
	RestoreGetSysVars(savedGetSysVars)
	SetPos( MaxRow()-1, 0 )
return (status:updated)
