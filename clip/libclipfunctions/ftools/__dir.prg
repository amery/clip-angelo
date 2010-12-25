function __Dir ( mask )
	local i, j, bflag:=.f., files, fname:={}
	local maxlen:=11, maxname:=11, cset:=csetatmupa(), eblock:=errorBlock({|x|break(x)})

	if empty(mask)
		mask="*.dbf"
		bflag=.t.
	endif

	files=directory(mask)

	for i=1 to len(files)
		maxlen=max(maxlen, len(files[i][1]) )
	next

	maxlen=min(maxlen, maxcol()-40)
	maxlen-=3
	for i=1 to len(files)
		aadd( fname, atadjust(".", files[i][1],maxlen) )
		maxname=max(maxname,len(fname[i]))
	next

	if !bflag
		for i=1 to len(files)
			? fname[i],str(files[i][2],10,0), dtoc(files[i][3]), files[i][4]
		next
		csetatmupa(cset)
		errorBlock(eblock)
		return
	endif

	? padc([Database files],maxname),[  # records],[ last update], [  size ]
	for i=1 to len(files)
		use (files[i][1]) new shared
		j= iif(neterr(),"   error  ",str(lastrec(),10,0) )
		? fname[i], j, dtoc(lupdate()), str(files[i][2],10,0)
		close
	next

	csetatmupa(cset)
	errorBlock(eblock)
return
