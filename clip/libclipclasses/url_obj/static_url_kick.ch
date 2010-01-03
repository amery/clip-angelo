static function url_kick()
	local i
	if ::__wget==NIL .or. lower(::protocol)=="file"
		return .f.
	endif
	::messages+=freadstr(::__wget,10000)
	::__eof := fileeof(::__wget)
	i=at("length:",lower(::messages))
	if ::length==0 .and. i>0 .and. i<len(::messages)-25
		::length:=val(strtran(substr(::messages,i+8,12),",",""))
	endif
	i:=rat("&\n",::messages)
	::status:=substr(::messages,i+1)
	i:=fopen(::fileName)
	::ready:=flength(i)
	fclose(i)
	if "0K -> " $ ::status
		::status:="reading "+alltrim(str(::ready))+" of "+alltrim(str(::length))+" bytes"
	endif
return .t.
