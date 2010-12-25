function loadsets(file)
	local fh, buf,i,n,nn,d,p, __set_names:=setNames()
	local error,eblock:=errorblock({|e|break(e)})
	fh:=fopen(file,FO_READ)
	if fh<0
		outlog(2,file+": open error:",ferrorstr())
		return .f.
	endif
	while !fileeof(fh)
		buf:=filegetstr(fh)
		buf:=alltrim(buf)
		if left(buf,1)=="#" .or. empty(buf)
			loop
		endif
		i := at(":",buf)
		if i==0
			outlog(2,file+": format error:",buf)
			loop
		endif
		d:=alltrim(substr(buf,1,i-1))
		p:=alltrim(substr(buf,i+1))
		if upper(substr(d,1,5))=="_SET_"
			n:=upper(substr(d,6))
			nn:=0
			for i in __set_names KEYS
				if __set_names[i]==n
					nn:=i
					exit
				endif
			next
			if nn > 0
				begin sequence
					outlog(2,"add set "+d+" as "+p,__FILE__,__LINE__)
					set(nn, &p)
				recover using error
					outlog(2,file+":error setting:",buf,__FILE__,__LINE__)
					end sequence
					loop
			endif
		endif
		outlog(2,"add set '"+d+"' as '"+toString(p)+"'",__FILE__,__LINE__)
		set(d,p)
	enddo
	fclose(fh)
	errorBlock(eblock)
return .t.
