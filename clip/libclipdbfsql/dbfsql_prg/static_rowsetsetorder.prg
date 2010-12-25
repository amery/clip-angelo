STATIC FUNCTION rowsetSetOrder(self,tagname)
	self:curorder := tagname
RETURN SQLSetOrder(self:rowset,tagname)
