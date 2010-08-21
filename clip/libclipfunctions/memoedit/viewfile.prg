function viewFile(fName, ntop, nleft, nbot, nright, mode, user_func,;
		  str_len, tab_size, bline, bpos, wline, wpos)

	local tobj,i,nkey,oldcolor:=setcolor(),scr:=savescreen(), lExit

	if !(valtype(fName) $ "CM")
		return .f.
	endif

	setcolor("0/7")

	ntop=iif(ntop==NIL,0,min(ntop, maxrow()))
	nbot=iif(nbot==NIL,maxrow(),min(nbot, maxrow()))
	nleft=iif(nleft==NIL,0,min(nleft, maxcol()))
	nright=iif(nright==NIL,maxcol(),min(nright, maxcol()))
	str_len=iif(str_len==NIL, TE_MAX_LEN_STRING, str_len)
	tab_size=iif(tab_size==NIL, TE_TABSIZE, tab_size)
	bline=iif(bline==NIL,1,bline)
	bpos=iif(bpos==NIL,1,bpos)
	wline=iif(wline==NIL,1,wline)
	wpos=iif(wpos==NIL,1,wpos)
	mode=iif(mode==NIL,.t.,mode)

	dbox(ntop,nleft,nbot,nright,,,fname,[Esc: Cancel])

	tobj=textEditNew(ntop+1,nleft+1,nbot-1,nright-1,setcolor())

	tobj:modeFormat := 2
	tobj:line   := iif(bline!=NIL, bline, tobj:line)
	tobj:pos    := iif(bpos!=NIL,  bpos,  tobj:pos)
	tobj:rowWin := iif(wline!=NIL, wline, tobj:rowWin)
	tobj:colWin := iif(wpos!=NIL,  wpos,  tobj:colWin)
	tobj:marginRight := str_len
	tobj:autoWrap := .t.
	tobj:tabSize:= iif(tab_size!=NIL,tab_size,tobj:tabSizw)
	tobj:loadFile(fName)
	ShowScoreboard()

	while .t.
		nKey:=inkey(0)
		if setkey(nKey)!=NIL
			eval(setkey(nKey),procname(),procline(),readvar())
			loop
		endif
		viewKeys(nkey,tobj,@lExit)
		if lExit
			exit
		endif
	enddo

	setcolor(oldcolor)
	restscreen(,,,,scr)
return .t.
