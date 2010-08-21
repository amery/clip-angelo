**************************************
function o2update(newData,oldData)
	local ret:=map(),k

	if valtype(oldData)!="O"; return newData; endif
	if valtype(newData)!="O"; return newData; endif

	for k in oldData keys
		ret[k] := oldData[k]
	next
	for k in newData keys
		ret[k] := newData[k]
	next
return ret
