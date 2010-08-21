**************************************
function Inherit(Obj)
	local NewObj := nil, k

	if valtype(Obj)!="O"; return nil; endif

	NewObj := map()
	for k in Obj keys
		if k != HASH_SUPER
			if valtype(Obj[k])=="B"
				NewObj[k] := Obj[k]
			else
				NewObj[k] := @Obj[k]
			endif
		endif
	next
	NewObj:super := Obj
return NewObj
