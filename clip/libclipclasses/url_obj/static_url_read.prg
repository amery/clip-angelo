static function url_read(buf,nbytes)
	local i,ret
	i:=fopen(::fileName,FO_READ)
	fseek(i,::pos,FS_SET)
	ret := fread(i,@buf,nbytes)
	::__feof := fileEof(i)
	::__dEof := (::__eof .and. ::__feof )
	::pos=fseek(i,0,FS_RELATIVE)
	fclose(i)
return ret
