static func __applykey(browse,nkey)
	local b, ret:=TBR_CONTINUE
	b := browse:setkey(nKey)
	if b != NIL
		ret := eval(b,browse,nkey)
	else
		b := browse:setkey(0)
		if b != NIL
			ret := eval(b,browse,nkey)
		else
			ret:=TBR_EXCEPTION
		endif
	endif
return ret
