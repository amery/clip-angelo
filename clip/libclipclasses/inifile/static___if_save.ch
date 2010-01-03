static function __IF_save(sFile)
	local nPart,nKey, fh
	local tmp1:={},i
	local tmp2:={},j
	::error:=""
	if sFile==NIL
		fh:=fcreate(::file)
	else
		fh:=fcreate(sFile)
	endif
	if fh<0
		::error := ::file+[: create error:]+ferrorstr()
		return .f.
	endif
	for nPart in ::data KEYS
		aadd(tmp1,::__names[nPart])
	next
	asort(tmp1)
	for i=1 to len(tmp1)
		fwrite(fh,DOS_CRLF)
		fwrite(fh,"["+tmp1[i]+"]"+DOS_CRLF)
		***
		tmp2:={}
		for nKey in ::data[tmp1[i]] KEYS
			aadd(tmp2,::__names[nKey])
		next
		asort(tmp2)
		for j=1 to len(tmp2)
			fwrite(fh,"&\t"+tmp2[j]+"&\t= ")
			fwrite(fh, __xDataToString(::data[tmp1[i]][tmp2[j]]) )
			fwrite(fh,DOS_CRLF)
		next
	next
	/*
	for nPart in ::data KEYS
		fwrite(fh,DOS_CRLF)
		fwrite(fh,"["+::__names[nPart]+"]"+DOS_CRLF)
		***
		for nKey in ::data[nPart] KEYS
			fwrite(fh,"&\t"+::__names[nKey]+"&\t= ")
			fwrite(fh, __xDataToString(::data[nPart][nKey]) )
			fwrite(fh,DOS_CRLF)
		next
	next
	*/
	fwrite(fh,DOS_CRLF)
	fclose(fh)
return .t.
