static func ClearGetSysVars()
	local saved:=map()

	saved:=clone(status)
	saved:readVar:= ReadVar( "" )

	status:KillRead := .f.
	status:BumpTop := .f.
	status:BumpBot := .f.
	status:LastExit := 0
	status:LastPos := 0
	status:ReadProcName := ""
	status:ReadProcLine := 0
	status:Updated := .f.
	status:countgets:=0
return (saved)
