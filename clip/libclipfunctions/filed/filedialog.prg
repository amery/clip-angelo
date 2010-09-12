function fileDialog(pDir, pMask, history, lCanReturnSelected)
static __ST_HIST
local gi, retvalue, scr, k, r, f, s, nkey:=1, gfocus, i
local drv, ll, curCol, old_dir := curDir()
local asel, aret

lCanReturnSelected := iif(lCanReturnSelected==NIL .or. valtype(lCanReturnSelected)!="L", .f., lCanReturnSelected)
fdp:current := ""

k := int(2*maxcol(.t.)/3)
if (k-CNT_COLUMNS+1)%CNT_COLUMNS > 0
	k--
endif
r := int(3*maxrow(.t.)/6)
nLeft := int((maxcol(.t.)-k)/2)+1
nRight := nLeft + k - 1
nTop := int((maxrow(.t.)-r)/2)+1
nBottom := nTop + r - 1

setcolor(DIALOG_COLORS)

pHistory := history

wopen(nTop-3, nLeft-1, nBottom+2, nRight+1)
wbox()
fdp:nLeft   := 0
fdp:nRight  := maxcol()
fdp:nTop    := 2
fdp:nBottom := maxrow()-1


retvalue := ""
fdp:mask := iif(empty(pmask), "*", pMask)

if !empty(pDir)
	dirchange(pDir)
endif

if pHistory == NIL
	if empty(__ST_HIST)
		pHistory := historyObj()
		pHistory:setsize(HISTORY_SIZE)
		pHistory:colorSpec := HISTORY_COLORS
		__ST_HIST := pHistory
	else
		pHistory := clone(__ST_HIST)
	endif
endif

pHistory:Lrow := fdp:nTop-1
pHistory:Lcol := fdp:nLeft+1
pHistory:Rrow := fdp:nBottom-2
pHistory:Rcol := fdp:nRight-2

fdp:length   := k
gi := ""

fdp:getobj  := getnew(fdp:nTop-2, fdp:nLeft,{|_1| iif(_1==NIL, gi,gi:=_1)}, "gi", "@kS"+alltrim(str(fdp:length-1)), "w+/b")
fdp:listobj := listitemnew(fdp:nTop, fdp:nLeft, fdp:nBottom, fdp:nRight, CNT_COLUMNS, DELIMITER, LIST_COLORS)
initItem(fdp)
fdp:getobj:setFocus()
fdp:getobj:killFocus()
fdp:getobj:varPut(fdp:current+fdp:mask+DOP)

showview(fdp)
dispbegin()
@ -1, maxcol() - 1 say "X"
@ fdp:nBottom, fdp:nLeft say "<" color "W+/W"
@ fdp:nBottom, fdp:nRight say ">" color "W+/W"
@ fdp:nTop-2, fdp:nRight say ">" color "W+/B"
dispend()

fdp:getobj:setFocus()
fdp:getobj:gotopos(len(alltrim(fdp:getobj:varGet()))+1)
gfocus := .t.
lenCol := {}
lItem := fdp:ListObj
ll := nLeft
for i:=1 to CNT_COLUMNS
	aadd(lenCol, {ll, ll+lItem:LenCol[i]})
	ll += lItem:LenCol[i]+1
next
fdp:lSelected := lCanReturnSelected

while nkey!=0
	nkey := Inkey(0, INKEY_ALL)
	if mLeftDown() .and. (mRow() == nTop-3) .and. (mCol() == nLeft+maxcol()-1)
		nKey := 0
		loop
	endif
	nKey := iif(gfocus, __get_handler(nKey, @gfocus, @retvalue), __list_handler(nKey, @gfocus, @retvalue))
enddo
//restscreen(0, 0, maxrow(.t.), maxcol(.t.), scr)
wclose()
dirchange(PATH_DELIM+old_dir)

if lCanReturnSelected .and. fdp:listobj:nSelected > 0
	asel := fdp:listobj:getSelected()
	aret := {}
	if !empty(asel)
		aeval(asel, {|x| aadd(aret, currDrive()+PATH_DELIM+curdir()+PATH_DELIM+x)})
		return aret
	endif
endif

return retvalue
