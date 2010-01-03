static function url_readStr(nbytes)
	local i,ret
	i:=fopen(::fileName,FO_READ)
	fseek(i,::pos,FS_SET)
	ret := freadstr(i,nbytes)
	::__feof := fileEof(i)
	::__dEof := (::__eof .and. ::__feof )
	::pos=fseek(i,0,FS_RELATIVE)
	fclose(i)
return ret
