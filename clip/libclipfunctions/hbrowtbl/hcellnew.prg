** Class HCell
function HCellNew(tag, align)
local obj:=map()
	obj:className	:= "HCELL"
	obj:items	:= {}
	obj:maxWidth	:= 1
	obj:minWidth	:= 1
	obj:Width	:= 1
	obj:colspan	:= 1
	obj:rowspan	:= 1
	obj:Height	:= 1
	obj:type	:= nil
	obj:align	:= iif(align==NIL, HASH_LEFT, align)
	obj:new		:= @tc_new_cell()
	obj:addItem	:= @tc_addCellItem()
	obj:setWidth	:= @tc_setWidthCell()

	if valtype(tag)!="U"
		obj:new(tag)
	endif
return obj
