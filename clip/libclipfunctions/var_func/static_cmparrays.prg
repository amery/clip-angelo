**************************************
static function cmpArrays(aOld,aNew)
	local i,x,t,vOld,vNew
	if len(aOld) != len(aNew)
		return .f.
	endif
	for i=1 to len(aOld)
		vNew := aNew[i]
		vOld := aOld[i]
		t := valtype(vNew)
		if t != valtype(vOld)
			return .f.
		endif
		if  t $ "CLDNM"
			if vOld != vNew
				return .f.
			endif
			loop
		endif
		if  t == "O"
			x := diffObj(vOld,vNew)
			if !empty(x)
				return .f.
			endif
			loop
		endif
		if  t == "A"
			x := cmpArrays(vOld,vNew)
			if !x
				return .f.
			endif
		endif
	next
return  .t.
