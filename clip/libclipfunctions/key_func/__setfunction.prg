function __setfunction(nKey,cStr)
	local fKey
	if nKey < 1 .or. nKey >40
		return .f.
	endif
	nkey--
	if nkey==0
		fkey:=K_F1
	else
		fkey:=0-nKey
	endif

	setkey(fkey,{||__keyboard(cStr) })

return .t.
