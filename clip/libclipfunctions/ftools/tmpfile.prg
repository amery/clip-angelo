function tmpfile()
	local f:=""
	while .t.
#ifdef __CLIP__
		f=PATH_DELIM+"tmp"+PATH_DELIM+"clip"+alltrim(str(random(10000),4,0))
#else
		f=currdrive()+"\"+curdir()+"\tmp\prom"+alltrim(str(random(10000),4,0))
#endif
		f=strtran(f," ","0")
		if !file(f+".dbf") .and. !file(f+".dbt") .and. !file(f+".txt")
			exit
		endif
	enddo
#ifdef __CLIP__
	 f=makepath(f)
#endif
return f
