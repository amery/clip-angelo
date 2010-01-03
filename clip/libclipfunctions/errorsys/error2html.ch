function error2html(err)
	local i
	if "ARGS" $ err
		for i=1 to len(err:args)
			if valtype(err:args[i]) == "O"
				err:args[i] := "{OBJECT}"
			endif
		next
	endif
	?? "Content-type: text/html"
	?
	?
	? "system error:",err
	? "<BR/>"
	i := 1
	while ( !Empty(ProcName(i)) )
		? "Called from", allTrim(ProcName(i)) + ;
			"(" + str(ProcLine(i)) + ")  <BR/>"

		i++
	end
	? "</RDF:RDF>"
	outlog("object error:",err)
	errorMessage(err)
quit
