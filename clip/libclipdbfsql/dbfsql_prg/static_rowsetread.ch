STATIC FUNCTION rowsetRead(self)
	LOCAL data := map()
	LOCAL I
	FOR I:=1 TO SQLNFields(self:rowset)
		data[HASHSTR(UPPER(SQLFieldName(self:rowset,I)))] := self:GetValue(I)
	NEXT
RETURN data
