**********************************************
* Begin new page printing
**********************************************
function printEject()
	qout("\eject\")
	setprc(0,0)
	print_queue:pageCount++
	/* begin real printing */
	if print_queue:begPage == print_queue:pageCount
		set(_SET_PRINTFILE,print_queue:outfile)
	endif
	/* end real printing */
	if print_queue:endPage == print_queue:pageCount
		set(_SET_PRINTFILE,"/dev/null")
	endif
return
