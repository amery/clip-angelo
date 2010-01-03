STATIC FUNCTION rowsetGoTop(self)
	_sqlflush(self)
	SQLGotop(self:rowset)
RETURN
