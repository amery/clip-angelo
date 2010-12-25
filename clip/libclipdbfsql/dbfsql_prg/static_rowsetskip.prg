STATIC FUNCTION rowsetSkip(self,rows)
	LOCAL ret
	IF rows==NIL;rows:=1;ENDIF

	_sqlflush(self)
	ret := SQLSkip(self:rowset,rows)
RETURN ret
