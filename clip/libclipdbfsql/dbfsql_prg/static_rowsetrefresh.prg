STATIC FUNCTION rowsetRefresh(self)
	LOCAL recno := SQLRecno(self:rowset)
	LOCAL stmt

	IF self:rowset != NIL
		SQLDestroyRowset(self:rowset)
	ENDIF
	stmt := SQLPrepare(self:conn,self:selectSQL)
	self:rowset := SQLCreateRowset(self,stmt,self:parameters,self:idname,self:orders)
	SQLFillOrders(self:rowset)
	self:SetOrder(self:curorder)
	SQLGoto(self:rowset,recno)
RETURN SQLLastrec(self:rowset)
