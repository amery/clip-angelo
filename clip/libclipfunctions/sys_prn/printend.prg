**********************************************
* end printing
**********************************************
function printEnd()
	local prnIni,vCodes
	local newFile, nCopy
	local sysOut:=space(0), sysErr:=space(0)

	if print_queue == NIL
		alert([PrintEnd(): printing process not actived])
		return .f.
	endif

	qout("\RESET\")
	set(_SET_CONSOLE,.t.)
	set(_SET_PRINTER,.f.)
	set(_SET_PRINTFILE,"")

	if upper(print_desc:type) == "VIRTUAL"
		vCodes:={}
	else
		prnIni:=iniFileNew(cliproot()+PATH_DELIM+"print"+PATH_DELIM+lower(print_desc:type))
		prnIni:Load()
		vCodes := prnIni:keys("vcodes")
	endif
	newFile := print_queue:outfile+".1"

	if empty(vCodes) .and. lower(host_charset()) == lower(print_desc:charset)
		newFile := print_queue:outfile
	else
		/* translate virtual codes to real printer codes */
		/* and charsets too */
		__translatePrintFile(print_queue:outfile, newFile, prnIni,print_desc:charset)
	endif

	for nCopy=1 to print_queue:copies
		if !empty(print_desc:block)
			eval(print_desc:block,newFile)
		endif
		if !empty(print_desc:script)
			if syscmd(print_desc:script+" "+newFile,"",@sysOut,@sysErr)!=0
				outlog(3,"Printing error",sysOut,sysErr)
				exit
			endif
		endif
	next

	fErase(print_queue:outfile)
	fErase(newFile)
	print_queue := NIL
	print_desc  := NIL
return  .t.
