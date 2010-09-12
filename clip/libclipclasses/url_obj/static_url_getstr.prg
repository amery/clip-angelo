static function url_getStr(nbytes)
	local i,ret
	i:=fopen(::fileName,FO_READ)
	fseek(i,::pos,FS_SET)
	ret := filegetstr(i,nbytes)
	::__feof := fileEof(i)
	::__dEof := (::__eof .and. ::__feof )
	::pos=fseek(i,0,FS_RELATIVE)
	fclose(i)
return ret
