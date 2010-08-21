**************************************
function varClone(var)
	local NewVar := nil, k, t:=valtype(var)
	if t == "O"
		NewVar := map()
		for k in Var keys
			newVar[k] := varClone(var[k])
		next
	elseif t == "A"
		newVar := {}
		for k=1 to len(var)
			aadd(newVar,varClone(var[k]))
		next
	else
		newVar := var
	endif
return NewVar
