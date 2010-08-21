STATIC FUNCTION rowsetDestroy(self)
	LOCAL conn := self:connect
	LOCAL p

	p := ASCAN(conn:rowsets,{|x| x == self:rowset } )
	IF p != 0
		ADEL(conn:rowsets,p)
		ASIZE(conn:rowsets,LEN(conn:rowsets)-1)
	ENDIF
	IF self:rowset != NIL
		SQLDestroyRowset(self:rowset)
		self:rowset := NIL
	ENDIF
RETURN
