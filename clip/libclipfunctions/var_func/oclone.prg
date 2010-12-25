**************************************
function oclone(Obj)
	local NewObj := nil, k,vt

	if valtype(Obj)!="O"; return nil; endif

	NewObj := map()
	for k in Obj keys
		vt := valtype(Obj[k])
		if vt=="O"
			newObj[k] := oClone(obj[k])
		elseif vt=="A"
			newObj[k] := aClone(obj[k])
		else
			NewObj[k] := Obj[k]
		endif
	next
return NewObj
