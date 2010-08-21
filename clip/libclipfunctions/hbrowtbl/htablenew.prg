** Class HTable
function HTableNew(selfDoc, width, border, parse)
local obj := selfDoc//map()
	obj:className	:= "HTABLE"
	obj:caption	:= NIL
	obj:captionAlign:= "TOP"
	obj:Titems	:= {}
	obj:Tcolumns	:= {}
	obj:Trows	:= {}
	obj:curCol	:= 0
	obj:curRow	:= 0
	obj:Twidth	:= 0
	obj:Theight	:= 0
	obj:posDoc	:= 0
	obj:rowDoc	:= 0
	obj:alignRow	:= HASH_LEFT
	obj:border	:= iif(border==NIL, .f., border)
	obj:PosInTable	:= 1

	_recover_HTABLE(obj)

return obj
