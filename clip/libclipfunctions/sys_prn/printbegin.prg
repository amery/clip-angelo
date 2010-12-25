**********************************************
* begin printing
**********************************************
function printBegin()
	local cp,pfile, oData, prn
	oData := loadPrinters()
	if print_queue != NIL
		alert([Printbegin(): printing already in process])
		return .f.
	endif
	prn:=printChoice(oData:queue, oData:printers)
	if prn <= 0
		return .f.
	endif
	pfile:=tmpfile()+".txt"
	ferase(pfile)
	oData:queue:outfile := pFile
	oData:queue:pageCount := 1
	print_queue := oData:queue
	print_desc  := oData:printers[prn]
	set(_SET_CONSOLE,.f.)
	set(_SET_PRINTER,.t.)
	if print_queue:begPage<=1
		set(_SET_PRINTFILE,pfile)
	else
		set(_SET_PRINTFILE,"/dev/null")
	endif
	setprc(0,0)
return .t.
