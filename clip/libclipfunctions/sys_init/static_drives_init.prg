static function drives_init(path)
	local ret:=.f., fh, buf, d,p,f
	f:=path+PATH_DELIM+"etc"+PATH_DELIM+"drives"
	if !file(f)
		outlog(2,"$CLIPROOT/etc don`t have file 'drives'")
		return .f.
	endif
	fh:=fopen(f,FO_READ)
	if fh<0
		outlog(2,"$CLIPROOT/etc/drives: open error:",ferrorstr())
		return .f.
	endif
	while !fileeof(fh)
		   buf:=filegetstr(fh)
		   buf:=alltrim(buf)
		   if left(buf,1)=="#" .or. empty(buf)
			loop
		   endif
		   if substr(buf,2,1)==":"
			d:=upper(substr(buf,1,1))+":"
			p:=alltrim(substr(buf,3))
			outlog(2,"add drive "+d+" as "+p,__FILE__,__LINE__)
			set(d,p)
			ret:=.t.
		   endif
	enddo
	fclose(fh)
return ret
