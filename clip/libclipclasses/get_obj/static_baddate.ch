static function badDate()
   local d,s,s1,s2,s3,s4,i,j, ret:=.f.
#ifdef DEBUG
	outlog(__FILE__,__LINE__,"baddate",::name)
#endif
	if ::buffer==NIL
		return .t.
	endif
	if ::type=="D"
	else
		return ret
	endif
	s := ""
	s4 := ::__dateFormat
	j := 1
	for i=1 to len(s4)
		s2 := substr(s4,i,1)
		if s2 $ DATE_DELIMITER_CHARS
			s += s2
		else
			s += substr(::__original,j,1)
			j ++
		endif
	next
	::typeOut := .f.
	s := alltrim(s)
	d := ctod(s,::__dateFormat)
	ret := ( !( s== alltrim(dtoc(d,::__dateFormat)) ) )
	if !ret
		  return ret
	endif
	s4 := strtran(upper(::__dateFormat),"YYYY","YY")
	d := ctod(s,s4)
	ret := ( !( s== alltrim(dtoc(d,s4)) ) )
#ifdef DEBUG
	outlog(__FILE__,__LINE__,"baddate",::name,"return",ret)
#endif
return ret
