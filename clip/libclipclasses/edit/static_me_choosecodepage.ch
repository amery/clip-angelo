************************************
static function me_chooseCodePage()
local pathCP:="", a, b, lo, nkey:=-1, str, ch, hch
local i,k, r, nLeft, nRight, nTop, nBottom, cntc:=4, item, oldcolor, owin

	pathCP += CLIPROOT()+PATH_DELIM+"charsets"
	b = {}
	a = fileseek(pathCP+PATH_DELIM+"*.tbl", FA_ARCHIVE+FA_VOLUME+FA_SYSTEM+FA_HIDDEN+FA_READONLY)
	while !empty(a)
		a := left(a, len(a)-4)
		aadd(b, a)
		a = fileseek()
	enddo
	b := asort(b,,,{|x, y| x < y})
	k := int(2*maxcol(.t.)/3)
	if (k-cntc+1)%cntc > 0
		k--
	endif
	r := int(3*maxrow(.t.)/6)
	nLeft := int((maxcol(.t.)-k)/2)+1
	nRight := nLeft + k - 1
	nTop := int((maxrow(.t.)-r)/2)+1
	nBottom := nTop + r - 1
	oldcolor := setcolor("15/7")
	owin := wselect()
	wopen(nTop, nLeft, nBottom, nRight)
	wbox()
	str := [ Source charset ]
	@ -1, (maxcol()-len(str))/2 say str
	lo := listitemnew(0, 0, maxrow(), maxcol(), cntc, substr(B_SINGLE, 4, 1), set("edit_colors_list"))
	for i in b
		lo:addItem(i)
	next
	lo:setFocus()
	lo:refresh()
	do while nkey!=0
		nkey := Inkey(0)
		i := lo:handleKey(nKey)
		if i == -1
			i := 0
			nKey := 0
		endif
		if i + nKey == 0
			loop
		endif
		switch(nkey)
		case K_ESC
			nkey := 0
		case K_ENTER
			item = alltrim(lo:getItem())
			nkey := 0
		otherwise
		    if nkey>=32 .and. nkey<=256
			lo:find(chr(nkey))
		    endif
	endswitch
	enddo
	wclose()
	wselect(owin)
	if item == NIL
		return
	endif
	::tobj[::curwin]:setCharset(item)
	/*
	if item != ::tobj[::curwin]:__hostcharset
		::tobj[::curwin]:charset := item
		ch := ::tobj[::curwin]:charset
		hch := ::tobj[::curwin]:__hostcharset
	else
		hch := ::tobj[::curwin]:charset
		ch := item
		::tobj[::curwin]:charset := item
	endif
	//i := ::tobj[::curwin]:edbuffer
	//aeval(i, {|_item| _item:=translate_charset(ch, hch, _item)})
	//::tobj[::curwin]:edbuffer := i
	aeval(::tobj[::curwin]:edbuffer, {|_item| i:=_item,_item:=translate_charset(ch, hch, _item)})
	for i=1 to ::tobj[::curwin]:lines
	outlog(__FILE__, ::tobj[::curwin]:edbuffer[i])
	next

/*	for i=1 to ::tobj[::curwin]:lines
	//	::tobj[::curwin]:edbuffer[i] := translate_charset(::tobj[::curwin]:charset, ::tobj[::curwin]:__hostcharset, ::tobj[::curwin]:edbuffer[i])
		::tobj[::curwin]:edbuffer[i] := translate_charset(ch, hch, ::tobj[::curwin]:edbuffer[i])
	next
*/

	::tobj[::curwin]:refresh()
	setcolor(oldcolor)
	*/
return
