static func editKeys(nkey,tobj,lExit, lData)
    local ret:=.t.,i, str, crlf
    lExit:=.f.
    do case

       case !EMPTY(lData)
	    if ( Set(_SET_INSERT) )
	       tobj:insert(chr(nkey))
	    else
	       tobj:overStrike(chr(nkey))
	    endif

       case nkey==K_ESC .and. tobj:updated

	     lExit:= .t. //alert([Exit from memoedit; Don~t save ?],{[Yes],[No ]})==1

       case nkey==K_CTRL_W .and. tobj:updated
	     crlf := set("CRLF_MEMOEDIT")
	     if empty(crlf)
		crlf := "&\r&\n"
	     endif
	     str:=""
	     for i=1 to tobj:lines
		if tobj:tabPack
			str+=tabPack(tobj:edBuffer[i],tobj:tabSize)
		else
			str+=tobj:edBuffer[i]
		endif
		if i<>tobj:lines
			str+="&\r&\n" //CRLF
		endif
	     next

	     lExit := str

       case nkey==K_ENTER
	    //if ( Set(_SET_INSERT) )
		tobj:insertLine()
	    //else
	    //	tobj:newLine()
	    //endif
       case nkey==K_DEL
	    tobj:delRight()
       case nkey==K_CTRL_T
	    tobj:delEnd()
       case nkey==K_BS
	    tobj:backSpace()
       case nkey==K_CTRL_BS
	    tobj:delHome()
       case nkey==K_CTRL_Y
	    tobj:deleteLine()
       case nkey==K_CTRL_F7
	    tobj:centerLine()
       case nkey==K_INS
	    set( _SET_INSERT, !Set(_SET_INSERT) )
	    ShowScoreboard()
       otherwise
	 if nkey>=0 .and. nkey<256
	    if ( Set(_SET_INSERT) )
	       tobj:insert(chr(nkey))
	    else
	       tobj:overStrike(chr(nkey))
	    endif
	 else
	    ret:=.f.
	 endif
    endcase
return ret
