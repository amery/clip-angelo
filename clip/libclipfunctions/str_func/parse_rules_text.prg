function parse_rules_text(sRules)
	local ret := {}, cTarget:=NIL,depends
	local m1,m2,m3,s,s1,s2,i,j,k
	m1 := split(sRules,"&\n")
	for i=1 to len(m1)
		s := m1[i]
		if left(s,1) == "&\t" .and. cTarget != NIL
			aadd(cTarget[3],substr(s,2))
			loop
		endif
		if empty(s) .or. left(s,1)=="#"
			loop
		endif
		m2 :=split(s,"[:, ]")
		if empty(m2)
			loop
		endif
		aadd(ret,{m2[1],{},{}})
		cTarget := atail(ret)
		for j=2 to len(m2)
			if empty(m2[j])
				loop
			endif
			aadd(ctarget[2],m2[j])
		next
	next
return ret
