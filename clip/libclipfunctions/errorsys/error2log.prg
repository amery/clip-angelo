function error2Log(err)
	local i,s
	if "ARGS" $ err
		for i=1 to len(err:args)
			if valtype(err:args[i]) == "O"
				err:args[i] := "{OBJECT}"
			endif
		next
	endif
	i := 1
	while ( !Empty(ProcName(i)) )
		s := "Called from "+allTrim(ProcName(i)) + ;
			"(" + str(ProcLine(i)) + ")"

		outlog(s)
		i++
	end
	outlog("object error:",err)
	s := errorMessage(err)
	? s
	outlog(s)
return
