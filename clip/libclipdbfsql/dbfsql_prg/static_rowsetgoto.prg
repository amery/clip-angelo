STATIC FUNCTION rowsetGoto(self,row)
	LOCAL ret

	_sqlflush(self)
	ret := SQLGoto(self:rowset,row)
RETURN ret
