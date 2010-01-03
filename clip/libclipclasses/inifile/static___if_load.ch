static function __IF_load()
	local fh, buf, ch, i, sKey,sVal, sPart:="", oPart
	::error:=""
	if ::file == NIL
		::error := [Bad file name: name is not specified]
		return .f.
	endif
	/*
	if ".ini" $ ::file
	else
		::file += ".ini"
	endif
	*/
	fh:=fopen(::file,FO_READ)
	if fh<0
		::error := ::file+[: open error:]+ferrorstr()
		return .f.
	endif
	while !fileeof(fh)
	       buf:=filegetstr(fh)
	       buf:=alltrim(buf)
	       ch:=left(buf,1)
	       i := at(";",buf)
	       if i>0
			buf:=substr(buf,1,i-1)
	       endif
	       if empty(buf) .or. ch=="#"
			loop
	       endif
	       if ch=="[" //begin new part
			if right(buf,1)=="]"
				sPart:=substr(buf,2,len(buf)-2)
			else
				sPart:=substr(buf,2,len(buf)-1)
			endif
			sPart := ::checkName(sPart)
			::__names[sPart]:=sPart
			::data[sPart] := map()
			oPart := ::data[sPart]
			loop
	       endif
	       i := at("=",buf)
	       if i<=0
			::error+=[Bad string in .ini file:]+buf+";"
			loop
	       endif
	       sKey:=substr(buf,1,i-1)
	       skey:=::checkName(sKey)
	       sVal:=substr(buf,i+1)
	       ::__names[sKey] := sKey
	       oPart[sKey]:=__stringToData(sVal)
	enddo
	fclose(fh)
return .t.
