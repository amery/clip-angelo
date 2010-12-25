FUNCTION memotran(str,s1,s2)
	local ret
	s1:=iif(s1==NIL,";",s1)
	s2:=iif(s2==NIL," ",s2)
	ret:=strtran(str,chr(13)+chr(10),s1)
return strtran(ret,chr(141)+chr(10),s2)
