**************************************
function diffObj(oOld,oNew)
	local diff:=map()
	local i,x,t,vOld,vNew
	if valtype(oOld)!="O" .or. valtype(oNew)!="O"
		return diff
	endif
	for i in oNew KEYS
		if ! (i $ oOLd)
			diff[i] := oNew[i]
			loop
		endif
		vNew := oNew[i]
		vOld := oOld[i]
		t := valtype(vNew)
		if t != valtype(vOld)
			diff[i] := vNew
			loop
		endif
		if  t $ "CLDNM"
			if vOld != vNew
				diff[i] := vNew
			endif
			loop
		endif
		if  t == "O"
			x := diffObj(vOld,vNew)
			if !empty(x)
				diff[i] := x
			endif
			loop
		endif
		if  t == "A"
			x := cmpArrays(vOld,vNew)
			if !x
				diff[i] := vNew
			endif
			loop
		endif
	next
return diff
