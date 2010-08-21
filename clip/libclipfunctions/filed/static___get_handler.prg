* handler for get-object
static function __get_handler(nKey, gfocus, retvalue)
local mCol, mRow, i, curCol, curRow, f, r, s, k, error
	if mLeftDown()
		mCol := mCol()
		mRow := mRow()
		if !(between(mCol, nLeft-1, nRight+1) .and.;
		     between(mRow, nTop-2, nBottom+1))
			return
		endif

		do case
		case mRow == nBottom .and. mCol == nLeft
			fdp:getobj:assign()
			fdp:getobj:killFocus()
			fdp:listobj:setFocus()
			gFocus := .f.
			nKey := K_UP
		case mRow == nBottom .and. mCol == nRight
			fdp:getobj:assign()
			fdp:getobj:killFocus()
			fdp:listobj:setFocus()
			gFocus := .f.
			nKey := K_DOWN
		case mRow == nTop-2 .and. mCol == nRight
			nKey := K_DOWN
		case mRow == nTop-2
			nKey := K_ENTER
		case mCol == nLeft-1
			nKey := K_TAB
		case mCol == nRight+1
			nKey := K_TAB
		case between(mRow, nTop, nBottom)
			curCol := lItem:Pos
			for i:=1 to CNT_COLUMNS
				if between(mCol, lenCol[i][1], lenCol[i][2])
					curCol := i
					exit
				endif
			next
			curRow := mRow - nTop

			nKey := K_TAB

			lItem:setItem(lItem:first + (curCol-1)*lItem:RowWin + curRow )
		endcase
	endif
	do case
		case nkey == K_CTRL_Y
			fdp:getobj:delEnd()
		case nkey == K_LEFT
			fdp:getobj:left()
		case nkey == K_RIGHT
			fdp:getobj:right()
		case nkey == K_DOWN
			s := pHistory:run()
			if !empty(s)
				pHistory:insert(s)
			else
				s := fdp:getobj:varGet()
			endif
			fdp:getobj:setFocus()
			fdp:getobj:killFocus()
			fdp:getobj:varPut(padr(s, 256))
			//fdp:getobj:assign()
			fdp:getobj:setFocus()
			fdp:getobj:gotopos(len(alltrim(fdp:getobj:varGet()))+1)
		case nkey == K_HOME
			fdp:getobj:home()
		case nkey == K_END
			fdp:getobj:gotopos(len(alltrim(fdp:getobj:varGet()))+1)
		case nkey == K_INS
			fdp:getobj:Insert()
		case nkey == K_DEL
			fdp:getobj:delRight()
		case nkey == K_BS
			fdp:getobj:backSpace()
		case nkey == K_TAB
			gfocus := !gfocus
			fdp:getobj:assign()
			fdp:getobj:killFocus()
			fdp:listobj:setFocus()
		case nkey == K_ESC
			nkey := 0
		case nkey == K_ENTER
			r := alltrim(fdp:getobj:buffer)
			r := strtran(r, '\', '/')
			r := strtran(r, '//', '/')
			BEGIN SEQUENCE
			ERRORBLOCK({|e| alert(e:description+";"+e:args[1], break(e))})
			f := fileattr(r)
			RECOVER USING error
			END SEQUENCE
			do case
			case rat("*", r) != 0
				s := rat("/", r)
				if s <= 0
					fdp:mask:= substr(r, 3)
					r := substr(r, 1, 2)+"/"
				else
					fdp:mask:= substr(r, s+1)
					r := substr(r, 1, s)
				endif
			    BEGIN SEQUENCE
			    ERRORBLOCK({|e| alert(e:description+";"+e:args[1])})
			    dirchange(r)
			    RECOVER USING error
				do while !file(r)
				dirchange("../")
				enddo
				r := curdir()
			    END SEQUENCE
				initItem(fdp)
			case f >= FA_ARCHIVE
				retvalue := r
				nkey := 0
			case f<FA_ARCHIVE .and. f>0
				r += "/"
				r := strtran(r, "//", "/")
				if dirchange(r) == 0
					initItem(fdp)
				endif
			otherwise
				retvalue := r
				nkey := 0
				//messagep("Can't open;"+r)
			endcase
			s := rtrim(r)
			pHistory:insert(s)

			showview(fdp)
			fdp:getobj:killFocus()
			fdp:listobj:setFocus()
			gfocus := .f.
		otherwise
		    if nkey>=32 .and. nkey<=256
			fdp:getobj:Insert(chr(nkey))
		    endif
	endcase
return nKey
