static function dbf_charset_init(path)
	local ret:=.f., fh, buf,f
	f:=path+PATH_DELIM+"etc"+PATH_DELIM+"dbfcharset"
	if !file(f)
		outlog(2,"$CLIPROOT/etc don`t have file 'dbfcharset'")
		return .f.
	endif
	fh:=fopen(f,FO_READ)
	if fh<0
		outlog(2,"$CLIPROOT/etc/dbfcharset: open error:",ferrorstr())
		return .f.
	endif
	while !fileeof(fh)
		   buf:=filegetstr(fh)
		   buf:=alltrim(buf)
		   if left(buf,1)=="#" .or. empty(buf)
				loop
		   endif
		   if !empty(buf)
				exit
		   endif
	enddo
	if !empty(buf)
		set("DBF_CHARSET",lower(buf))
		outlog(2,"set DBF_CHARSET:",lower(buf),__FILE__,__LINE__)
		ret:=.t.
	endif
	fclose(fh)
return ret
