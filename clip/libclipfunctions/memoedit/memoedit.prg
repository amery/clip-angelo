function memoedit(str, ntop, nleft, nbot, nright, mode, user_func,;
		  str_len, tab_size, bline, bpos, wline, wpos)

	local tobj,i,nkey,oldcolor:=setcolor(), status
	local user_ret,row,col

	if !(valtype(str) $ "CM")
		str=""
	endif
	str := strtran(str,chr(13),"")

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

	tobj=textEditNew(ntop,nleft,nbot,nright,setcolor())

	tobj:modeFormat := 2
	tobj:line   := iif(bline!=NIL, bline, tobj:line)
	tobj:pos    := iif(bpos!=NIL,  bpos,  tobj:pos)
	tobj:rowWin := iif(wline!=NIL, wline, tobj:rowWin)
	tobj:colWin := iif(wpos!=NIL,  wpos,  tobj:colWin)
	tobj:marginRight := str_len
	tobj:autoWrap := .t.
	tobj:tabSize:= iif(tab_size!=NIL,tab_size,tobj:tabSizw)
	tobj:idletime:=60
	tobj:loadString(@str)

	if !empty( user_func ) .and. valtype( user_func ) == "C"
		user_ret := 1
		while   user_ret # 0
			row := row(); col:=col()
			user_ret:=clip(user_func, ME_INIT, tobj:line, tobj:pos-1, tobj, nil)
			setpos(row,col)
			user_action(tobj, user_ret, mode)
		enddo
	else
		user_func := NIL
	endif

	ShowScoreboard()

	while .t.
		if nextkey() = 0
			if !empty( user_func )
				row := row(); col:=col()
				user_ret := clip(user_func, ME_IDLE, tobj:line , tobj:pos-1, tobj, nil)
				setpos(row,col)
				user_ret:=user_action(tobj, user_ret, mode)
				if user_ret
					exit
				endif
			endif
		endif
		if (nKey:=inkey(tobj:idletime)) == 0
			loop
		endif
		if setkey(nKey)!=NIL
			eval(setkey(nKey),procname(),procline(),readvar())
			loop
		endif
		if empty(user_func)
		    if ! viewKeys(nKey,tobj,@user_ret) .and. mode
			editKeys(nKey,tobj,@user_ret)
		    endif
		else
		    tobj:cargo:=NIL
		    /* comment - if !empty(user_func) --> simple chars not writen*/
		    /* => status will be = ME_IDLE */
		    /* status:=ME_IDLE */
		      /* uncomment for Bondar */
		    status:=if(tobj:updated, ME_UNKEYX, ME_UNKEY)
		    row := row(); col:=col()
		    status:=clip(user_func, status, tobj:line , tobj:pos-1, tobj)
		    setpos(row,col)
		    if tobj:cargo<>NIL
			nkey:=tobj:cargo
		    endif
		    user_ret:=user_action(tobj, status, mode, nkey)
		endif
		if valtype(user_ret)=='C'
			str:=user_ret
			user_ret:=.t.
		endif
		if user_ret
			exit
		endif
	enddo
	setcolor(oldcolor)
return str
