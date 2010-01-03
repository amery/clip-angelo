STATIC PROCEDURE ApplyKey(browse, nKey)
    local ret:=.f. ,b
    b := browse:setkey(nKey)
    if b != NIL
	eval(b,browse,nkey)
	ret := .t.
    endif
RETURN ret
