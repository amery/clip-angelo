function _recover_HTABLE(obj)
	obj:addCellToTable	:= @tt_addCellToTable()
	obj:addRowToTable	:= @tt_addRowToTable()
	obj:addItemToCell	:= @tt_addItemToCell()
	obj:setCell		:= @tt_setCell()
	obj:drawTable		:= @tt_drawTable()
return obj
