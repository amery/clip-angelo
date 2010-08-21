function mimeTypesLoad(fName)
	local fh,buf, i,j,k

	types := map()

	if empty(fName)
		fname := "/etc/mime.types"
	endif

	fh:=fopen(fName,FO_READ)
	if fh<0
		return .f.
	endif
	while !fileeof(fh)
		buf:=filegetstr(fh)
		buf:=alltrim(buf)
		if left(buf,1)=="#" .or. empty(buf)
			loop
		endif
		buf := strtran(buf,"&\t"," ")
		i := at(" ",buf)
		if i<=0
			loop
		endif
		k := left(buf,i-1)
		buf := alltrim(substr(buf,i+1))
		while !empty(buf)
			i:=at(" ",buf)
			if i<=0
				i := len(buf)+1
			endif
			j := upper(left(buf,i-1))
			if empty(j)
				loop
			endif
			types[j] := k
			buf := alltrim(substr(buf,i+1))
		enddo
	enddo
	fclose(fh)
return .t.
