function keysend(sData,lAppend)
	lAppend := iif(valtype(lAppend) != "L", .f., lAppend )
	if lAppend
		t_buffer += sData
	else
		t_buffer := sData
		__keyboard("")
	endif
return .t.
