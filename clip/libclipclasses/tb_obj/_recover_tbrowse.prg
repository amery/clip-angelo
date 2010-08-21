function _recover_tbrowse(obj)
	obj:setKey	:=@tb_setKey()
	obj:applyKey	:=@__applykey()
	obj:down	:=@down()
	obj:up	:=@up()
	obj:left	:=@cleft()
	obj:right	:=@cright()
	obj:goBottom	:=@goBottom()
	obj:goTop	:=@goTop()
	obj:home	:=@home()
	obj:end	:=@end()
	obj:pageDown	:=@pageDown()
	obj:pageUp	:=@pageUp()
	obj:panHome	:=@panHome()
	obj:panEnd	:=@panEnd()
	obj:panLeft	:=@panLeft()
	obj:panRight	:=@panRight()

	obj:addColumn	:=@addColumn()
	obj:insColumn	:=@insColumn()
	obj:setColumn	:=@setColumn()
	obj:delColumn	:=@delColumn()
	obj:getColumn	:=@getColumn()
	obj:__delAllColumns	:=@__delAllColumns()

	obj:deHilite 	:=@deHilite()
	obj:hilite   	:=@hilite()

	obj:hitTest   	:=@tb_hitTest()

	obj:colorRect	:= @colorRect()
	obj:colWidth 	:= @colWidth()
	obj:configure	:= @configure()
	obj:invalidate	:= @invalidate()
	obj:refreshAll	:= @refreshAll()
	obj:refreshCurrent	:= @refreshCurrent()
	obj:refreshCurent	:= @refreshCurrent()
	obj:refreshCur	:= @refreshCurrent()
	obj:_outCurrent  	:= @_outCurrent()
	obj:stabilize	:= @stabilize()
	obj:forceStable	:= @forceStable()

	obj:__whoVisible	:= @__whoVisible() // ��������� ��� �������
	obj:__reMakeColumns	:= @__remakeColumns()
	obj:__dummyRow	:= @__dummyRow()
	obj:__sayTable	:= @__sayTable() // �������� ����������, ���������, �������
	obj:__setColor	:= @__setcolor()
	obj:__getColor	:= @__getcolor()
//	obj:__checkRow	:= @__checkRow()
	obj:__lenColSep := @__lenColSep()


	tb_set_default_keys(obj)

return obj
