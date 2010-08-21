function __WAIT(xMsg)
	local nKey, bBlock
	if xMsg==NIL
		? "Press any key to continue..."
	else
		? xMsg
	endif
	do while (.T.)
		nKey:= InKey(0)
		if ((bBlock:= SetKey(nKey)) != Nil)
			eval(bBlock, procname(1), procline(1), "")
			loop
		endif
		if (nKey >= 32 .AND. nKey <= 255)
			?? Chr(nKey)
		else
			nKey:= 0
		endif
		exit
	enddo
return Chr(nKey)
