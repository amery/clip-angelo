function tempfile(cDir,cExt,nAttr)
	local ret:="",i,s1,s2
	if valtype(cDir)=="C"
		s1 := makepath(cDir)
	else
		s1 := makePath("")
	endif
	if ! (right(s1,1) $ "\/")
	     s1 += PATH_DELIM
	endif
	cExt := iif(valtype(cExt)=="C",cExt,"")
	do while .t.
		ret := "ctmp"+lower(ntoc(random(0xFFFF),32,4,"0"))+'.'+cExt
		s2 := s1+ret
		s2 := makepath(s2)
		if !file(s2)
			exit
		endif
	enddo
	i := fcreate(s2)
	if i<0
		s2 := ""
	else
		fclose(i)
	endif
	if valtype(cDir)=="C"
		i := atr(cDir+PATH_DELIM,s2)
		if i>0
			s2 := substr(s2,i)
		endif
	else
		s2 := ret
	endif
return  s2
