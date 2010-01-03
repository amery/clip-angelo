function __DbList(off, fields, all, ufor, uwhile, unext, rec, rest, toPrint, toFile)
	local i,margin:=space(set(25)), count:=0
	local __bfor,__bwhile
	outlog(3,"list",alias(),"fields",fields,"for",ufor,"while",uwhile,"next",unext,"record",rec,"rest",rest,tofile)

	off:=iif(off==NIL,.t.,off)

	if valtype(rec)=="N"
		goto rec
		rest=.t.
		unext=1
	endif

	if uwhile != NIL
		rest := .t.
	endif

	if valtype(ufor)=="C"
		__bfor:="{||"+ufor+"}"
		__bfor:=&__bfor
	else
		if valtype(ufor)!="B"
			__bfor:={||.t.}
		else
			__bfor:=ufor
		endif
	endif

	if valtype(uwhile)=="C"
		__bwhile:="{||"+uwhile+"}"
		__bwhile:=&__bwhile
	else
		if valtype(uwhile)!="B"
			__bwhile:={||.t.}
		else
			__bwhile:=uwhile
		endif
	endif

	if empty(rest) .and. unext==NIL //rest==NIL .or. !rest
		goto top
	endif
	unext=iif(valtype(unext)!="N",lastrec(),unext)

	if !empty(toFile)
		if !("." $ toFile)
			toFile += ".prn"
		endif
		set alternate to (toFile)
		set alternate on
	endif

	do while !eof() .and. count<unext .and. eval(__bwhile)
		count++
		if eval(__bfor)
			? iif(off,margin,margin+str(recno()))
			?? iif(deleted(),"*"," ")
			for i=1 to len(fields)
				?? eval(fields[i]),""
			next
		endif
		if count<unext
			skip
		endif
	enddo
	if !empty(toFile)
		set alternate off
		set alternate to
	endif
return count
