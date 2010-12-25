* handler for list-object
static function __list_handler(nKey, gfocus, retvalue)
local mCol, mRow, i, curCol, curRow, f, r, s, k, item, error
	if mLeftDown()
		mCol := mCol()
		mRow := mRow()
		if !(between(mCol, nLeft-1, nRight+1) .and.;
		     between(mRow, nTop-2, nBottom+1))
			return
		endif

		do case
		case mRow == nBottom .and. mCol == nLeft
			nKey := K_UP
		case mRow == nBottom .and. mCol == nRight
			nKey := K_DOWN
		case mRow == nTop-2 .and. mCol == nRight
			fdp:listobj:killFocus()
			fdp:getobj:setFocus()
			gfocus := .t.
			nKey := K_DOWN
		case mRow == nTop-2
			nKey := K_TAB
		case mCol == nLeft-1
			nKey := K_LEFT
		case mCol == nRight+1
			nKey := K_RIGHT
		case between(mRow, nTop, nBottom)
			curCol := lItem:Pos
			for i:=1 to CNT_COLUMNS
				if between(mCol, lenCol[i][1], lenCol[i][2])
					curCol := i
					exit
				endif
			next
			curRow := mRow - nTop

			if lItem:first + (curCol-1)*lItem:RowWin + curRow == lItem:buffer
				nKey := K_ENTER
			endif

			lItem:setItem(lItem:first + (curCol-1)*lItem:RowWin + curRow )
		endcase
	endif
	if mRightDown()
		mCol := mCol()
		mRow := mRow()
		if !(between(mCol, nLeft-1, nRight+1) .and.;
		     between(mRow, nTop-2, nBottom+1))
			return
		endif

		if between(mRow, nTop, nBottom)
			curCol := lItem:Pos
			for i:=1 to CNT_COLUMNS
				if between(mCol, lenCol[i][1], lenCol[i][2])
					curCol := i
					exit
				endif
			next
			curRow := mRow - nTop

			lItem:setItem(lItem:first + (curCol-1)*lItem:RowWin + curRow )
			lItem:select()
			nKey := -1

		endif
	endif
	do case
		case nkey == K_LEFT
			fdp:listobj:left()
			showview(fdp)
		case nkey == K_RIGHT
			fdp:listobj:right()
			showview(fdp)
		case nkey == K_UP
			fdp:listobj:up()
			showview(fdp)
		case nkey == K_DOWN
			fdp:listobj:down()
			showview(fdp)
		case nkey == K_HOME
			fdp:listobj:home()
			showview(fdp)
		case nkey == K_END
			fdp:listobj:end()
			showview(fdp)
		case nkey == K_PGUP
			fdp:listobj:pageup()
			showview(fdp)
		case nkey == K_PGDN
			fdp:listobj:pagedown()
			showview(fdp)
		case nkey == K_INS
			fdp:listobj:select()
			fdp:listobj:down()
			//showview(fdp)
		case nkey == K_TAB
			gfocus := !gfocus
			fdp:listobj:killFocus()
			fdp:getobj:setFocus()
			fdp:getobj:gotopos(len(alltrim(fdp:getobj:varGet()))+1)
		case nkey == K_ESC
			nkey := 0
			fdp:listobj:nSelected := 0
		case nkey == K_ENTER .and. fdp:listobj:nSelected > 0
			nKey := 0
		case nkey == K_ENTER
			item = alltrim(fdp:listobj:getItem())
			k=rat("/", item)
			if k != 0
			    r:=lastdir(fdp:getobj:varGet())

			    if dirchange(substr(item, 1, k-1)) < 0
				alert([Bad directory name!])
				dirchange("/")
			    endif
			    initItem(fdp)
			    if item == "../"
				fdp:listobj:find(r)
				fdp:listobj:clearFindBuffer()
			    endif
			    showview(fdp)
			else
			    gfocus := .t.
			    fdp:listobj:killFocus()
			    fdp:getobj:setFocus()
			    fdp:getobj:killFocus()
			    fdp:getobj:varPut(padr(alltrim(fdp:current) + item, 256))
			    fdp:getobj:setFocus()
			    fdp:getobj:gotopos(len(alltrim(fdp:getobj:varGet()))+1)
			endif
		otherwise
		    if nkey>=32 .and. nkey<=256
			fdp:listobj:find(chr(nkey))
			i := fdp:listobj:buffer
			@ fdp:nBottom, fdp:nLeft say padr(substr(fdp:viewitem[i], 1, at("|", fdp:viewitem[i])-1), fdp:length)
			@ fdp:nBottom+1, fdp:nLeft say padr(substr(fdp:viewitem[i], at("|", fdp:viewitem[i])+1), fdp:length)
			devpos(fdp:listobj:line, fdp:listobj:col)
		    endif
	endcase
return nKey
