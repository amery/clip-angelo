function saveSets()
	local i,__set_names:=setnames()
	for i in __set_names KEYS
	      __set_names[i]:=set(i)
	next
return __set_names
