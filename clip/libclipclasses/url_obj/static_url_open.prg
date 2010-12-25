static function url_open()
	local i,j:=1,s

	::__eof	:= .f.
	::__dEof:= .f.
	::__fEof:= .f.

	::start := seconds()
	if lower(::protocol)=="file"
		::filename=::path+"/"+::file
		::status:="read local file"
		::__eof:=.t.
		i:=fopen(::fileName)
		::ready:=flength(i)
		::length:=::ready
		fclose(i)
		return i>=0
	endif
	::fileName := getenv("HOME")+"/.clip/cache.web/"+::protocol+"/"+::host+::path+"/"+::file
	while ( i:=at("/",::fileName,j) )>0
		s=substr(::filename,1,i-1)
		dirMake(s)
		j++
	enddo
#ifndef WGET_PRG
	outlog(__FILE__, __LINE__, "program wget not installed")
	return .f.
#endif
	s= WGET_PRG+" -c -v -s -O "+::fileName+" "
	s+=::makeUrl(.f.)
	::__wget:=fopen(s+" |",FO_READ+FO_NOBUFF)
	if ferror()!=0 .or. ::__wget<0
		return .f.
	endif
return .t.
