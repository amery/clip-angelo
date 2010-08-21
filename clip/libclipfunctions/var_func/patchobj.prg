**************************************
function patchObj(obj,diff)
	local i
	if valtype(obj)!="O" .or. valtype(diff)!="O"
		return obj
	endif
	for i in diff KEYS
		obj[i] := diff[i]
	next
return obj
