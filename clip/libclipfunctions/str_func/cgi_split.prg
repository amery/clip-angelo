FUNCTION cgi_split(cgistr,aType)
	local c,arr, le, i, k, s, key, val, iskey := .f., prevs:=""
	if aType == NIL
		aType :=.f.
	endif
	if aType
		arr:={}
	else
		arr:=map()
	endif
	if empty(cgistr)
		return arr
	endif

	le := len(cgistr)
	key := ""
	val := ""
	k := ""
	for i=1 to le+1
		s := substr(cgistr, i, 1)
		do case
		case s == "=" .and. ! ( prevs $ "<>" )
			key := k
			k := ""
			iskey := .t.
		case s == "&" .or. s== ";"
			if iskey
				val := k
			else
				key := k
			endif
			if aType
				aadd(arr,{key,val})
			else
				arr[hashstr(upper(key))] := val
			endif
			iskey := .f.
			key := ""
			val := ""
			k:=""
		case s == "%"
			c := substr(cgistr, i+1, 2)
			if len(c)==2
				k += chr(cton(c, 16))
			endif
			i += len(c)
		case s == "+"
			k += " "
		otherwise
			k += s
		endcase
		prevs := s
	next
	if iskey
		val := k
	else
		key := k
	endif
	if .not. empty(key)
	  if aType
		aadd(arr, {key, val})
	  else
		arr[hashstr(upper(key))] := val
	  endif
	endif
return arr
