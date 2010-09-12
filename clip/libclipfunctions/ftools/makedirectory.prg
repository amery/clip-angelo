function makeDirectory(path,mode)
	local curdir:=curDir()
	local r3, p:=path, i, s, q,qq

	if empty(mode)
		mode := "777"
	endif
	if dirChange(path) >= 0
		dirChange(PATH_DELIM+curDir)
		return .t.
	endif
	if left(p,1) == PATH_DELIM
#ifdef OS_CYGWIN
		qq := currDrive()+PATH_DELIM+"cygwin"+PATH_DELIM
#else
		qq := PATH_DELIM
#endif
		dirChange(qq)
	endif
	while !empty(p)
		i:=at(PATH_DELIM,p)
		i:=iif(i==0,len(p)+1,i)
		s:=substr(p,1,i-1)
		p:=substr(p,i+1)
		if dirMake(s)<0 .and. empty(p)
			dirChange(PATH_DELIM+curDir)
//			qq += PATH_DELIM+curDir
			return .f.
		endif
		//chmod(s,mode)
		q := s
		r3:=dirChange(s)
	enddo
//	outlog(__FILE__,__LINE__,q,mode)
//	if !empty(q)
//		chmod(q,mode)
//	endif
	chmod(path,mode)
	dirChange(PATH_DELIM+curDir)
return .t.
