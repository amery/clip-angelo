STATIC FUNCTION connDestroy(self)
	LOCAL I
	LOCAL rs

	IF self:conn != NIL
		FOR I:=1 TO LEN(self:Rowsets)
			rs := self:Rowsets[I]
			SQLDestroyRowset(rs)
		NEXT
		SQLDestroyConn(self:conn)
		self:conn := NIL
		self:Rowsets := NIL
	ENDIF
RETURN
