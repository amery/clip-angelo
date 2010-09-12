static function run(current, retitem)
local ret:="", lb, nkey:=1, length, scr, s, i

current := iif(current == NIL, 1, current)
retitem := iif(retitem == NIL, .t., .f.)
s := setcolor(::colorSpec)
save screen to scr
length := ::Rcol-::Lcol-4
@ ::Lrow, ::Lcol clear to ::Rrow, ::Rcol
@ ::Lrow, ::Lcol TO ::Rrow, ::Rcol color ::colorSpec
if !empty(::header)
	i := int(3*(::Rcol-::Lcol)/4)
	@ ::Lrow, ::Lcol+(i-len(::header))/2 say ::header
endif
lb := listitemnew(::Lrow+1, ::Lcol+1, ::Rrow-1, ::Rcol, 1, "", ::colorSpec)
for i=1 to len(::history)
	if ::history[i] != NIL
		lb:addItem(padr(::history[i], length))
	endif
next
lb:setFocus()
lb:setItem(current)
while nkey != 0
	nkey := inkey(0)
	do case
	case nkey == K_ESC
		if retitem
			ret := ""
		else
			ret := 0
		endif
		nkey := 0
	case nkey == K_ENTER
		if lb:itemCount>0
			ret := iif(retitem, rtrim(::history[lb:buffer]), lb:buffer)
		endif
		nkey := 0
	case nkey == K_DOWN
		lb:Down()
	case nkey == K_UP
		lb:Up()
	case nkey == K_HOME
		lb:Home()
	case nkey == K_HOME
		lb:End()
	endcase
enddo
setcolor(s)
restore screen from scr
return ret
