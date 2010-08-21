***********************************************
* choice printer
***********************************************
function printChoice(oQueue, aPrinters)
	local oldcol:=col(),oldrow:=row()
	local ret:=0, nT, nL, nB, nR
	local cp:=1,bl:=1,el:=9999,i,s,x1,x2,x3
	local nkey:=0,scr,oldcolor,getlist:={}
	save screen to scr
	oldcolor=setcolor("W/R")
	do while .t.
		nb:= maxrow()- 2
		nT:= nb - len(aprinters) - 6
		nL:= 15
		nR:= maxcol()-nl
		dbox(nT, nL-1, nB, nR,,, [choice printer], [Esc: Cancel] )
		nt++
		bl:=oQueue:begPage
		el:=oQueue:endPage
		cp:=oQueue:copies
		for i=1 to len(aPrinters)
			@ ++nT,nL+1 say str(i,3,0) color "W+/R"
			dispout(": "+padr(aPrinters[i]:description, nr-nl-6))
		next
		@ ++nT,nL+1 say replicate("-",nr-nl-2)
		@ ++nT,nL+1 say "Ins" color "W+/R"
		s:=[: Printing pages range: from XXXX to YYYY]
		x1:=col()+at("XXXX",s)-1
		x2:=col()+at("YYYY",s)-1
		s:=strtran(s,"XXXX",str(bl,4,0))
		s:=strtran(s,"YYYY",str(el,4,0))
		dispout(s)
		@ ++nT,nL+1 say "Del" color "W+/R"
		s:=[: Printing copies: XXXX ]
		x3:=col()+at("XXXX",s)-1
		s:=strtran(s,"XXXX",str(cp,4,0))
		dispout(s)
		nkey:=inkey(0)
		if setkey(nKey) != NIL
			eval(setkey(nKey),procname(),procline(),readvar())
			loop
		endif
		if nKey==K_ENTER
			nkey := asc("1")
		endif
		if nkey==K_INS
			@ nT-1, x1 get bl picture "9999"
			@ nT-1, x2 get el picture "9999"
			read
			oQueue:begPage := bl
			oQueue:endPage := el
			clearkey()
			loop
		endif
		if nkey==K_DEL
			@ nT, x3 get cp picture "9999"
			read
			oQueue:copies := cp
			clearkey()
			loop
		endif
		if nkey==K_ESC
			exit
		endif
		if nKey >= asc("1") .and. nkey <= asc("9")
			ret:= nKey - asc("0")
			exit
		endif
	enddo
	keyboard chr(0)
	inkey(0.1)
	restore screen from scr
	setcolor(oldcolor)
	setpos(oldrow,oldcol)
return ret
