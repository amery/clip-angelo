**************************************
function varChanged(newData,oldData)
	local i,m1,m2,val1,val2
	local t1:=valtype(newData), t2:=valtype(oldData)
	if t1 == t2
		if newData == oldData
			return .f.
		endif
	else
		return .t.
	endif
	if t1 == "A"
		if len(newData) != len(oldData)
			return .t.
		endif
		for i=1 to len(newData)
			if varChanged(newData[i],oldData[i])
				return .t.
			endif
		next
		return .f.
	endif
	if t1 != "O" .or. t2!="O"
		return .t.
	endif
	m1 := mapkeys(newData)
	m2 := mapkeys(oldData)
	if len(m1) != len(m2)
		return .t.
	endif
	for i=1 to len(m1)
		if m1[i] != m2[i]
			return .t.
		endif
		val1 := newData[m1[i]]
		val2 := oldData[m2[i]]
		if varChanged(val1,val2)
			return .t.
		endif
	next
return .f.
