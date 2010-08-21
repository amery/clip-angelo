static proc RestoreGetSysVars(saved)
	local up:=status:updated
	status:=clone(saved)
	GetActive( saved:ActiveGet )
	ReadVar( saved:readVar )
	status:updated:=up
return
