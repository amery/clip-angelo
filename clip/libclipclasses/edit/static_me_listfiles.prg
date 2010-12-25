**********************************************************
static function me_listfiles()
local sv, fd, i, retvalue, scr, nKey:=1, listobj, length, cur, aa


length := maxcol() - 1

retvalue := ::curwin
listobj := listitemnew(0, 0, maxrow()+1, maxcol()+1, 1, , set("edit_colors_list"))
listobj:clear()
for i=1 to ::nWins
	if ::tobjinfo[i] == NIL
		loop
	endif
	listobj:addItem(padr(alltrim(str(::tobjinfo[i]:curwin))+ ;
	    iif(::tobjinfo[i]:readOnly, [R:], iif(::tobj[i]:updated, "*: ", " : "))+ ;
	    ::tobjinfo[i]:name, length))

	if ::tobjinfo[i]:curwin == retvalue
		cur := i
	endif
next
listobj:setfocus()
listobj:setItem(::curwin)

do while nkey!=0
	nkey := Inkey(0)
	i := listobj:handleKey(nKey)
	if i == -1
		i := 0
		nKey := 0
	endif
	if i + nKey == 0
		loop
	endif

	do case
		case nkey == K_DEL
			if listobj:itemCount < 1
				loop
			endif
			if ::nWins>0 .and. ::tobj[listobj:buffer]:updated
				sv := alert(::tobjinfo[listobj:buffer]:name+[; not saved. Save?], {[Yes], [No], [Cancel]}, set("edit_colors_menu") )
				if sv == 1
				    if empty(::tobjinfo[listobj:buffer]:name)
					fd=filedialog(__CurrentDir, "*", ::__FileD)
					if !empty(fd)
						__CurrentDir := padr(fd, atr(PATH_DELIM, fd))
					endif
					if !file(fd) .or. ;
					   (file(fd) .and. alert([File;]+fd+[;already exists. Overwrite it?], {[Yes], [No]}, set("edit_colors_menu")) == 1)
						::tobjinfo[listobj:buffer]:name := fd
						::tobj[listobj:buffer]:saveFile(fd)
					endif
				    endif
				endif
				if sv == 3
					loop
				endif
			endif
			cur := listobj:buffer
			dispbegin()
			aa:=wselect()
			wselect(::tobjinfo[cur]:curwin)
			wclose()
			wselect(aa)
			//::drawhead()
			dispend()

			adel(::tobj, listobj:buffer)
			asize(::tobj, ::nWins-1)
			adel(::tobjinfo, listobj:buffer)
			asize(::tobjinfo, ::nWins-1)
			::nWins--
			if ::curwin >= listobj:buffer
				::curwin --
			endif

			listobj:delItem(listobj:buffer)
			retvalue := listobj:buffer
		case nkey == K_ENTER
			if listobj:itemCount < 1
				retvalue := 0
			else
				retvalue := listobj:buffer
			endif
			nkey := 0
		otherwise
		    if nkey>=32 .and. nkey<=256
			listobj:find(chr(nkey))
			devpos(listobj:line, listobj:col)
		    endif
	endcase
enddo
//restore screen from scr
return retvalue
