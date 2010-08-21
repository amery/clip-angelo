function adir(mask,files,lens,dates,times,atrs)
	local len,m,i
	m=directory(mask, iif(pcount()>5,"D","") )
	len=len(m)
	if pcount()==1
		return len
	endif
	len=min(len,len(files))
	//asize(files,len)
	if lens==NIL
		lens:={}
	endif
	if dates==NIL
		dates:={}
	endif
	if times==NIL
		times:={}
	endif
	if atrs==NIL
		atrs:={}
	endif
	asize(lens,len)
	asize(dates,len)
	asize(times,len)
	asize(atrs,len)
	for i=1 to len
		files[i]=m[i][F_NAME]
		lens[i] =m[i][F_SIZE]
		dates[i]=m[i][F_DATE]
		times[i]=m[i][F_TIME]
		atrs[i] =m[i][F_ATTR]
	next
return len(m)
