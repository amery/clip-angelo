STATIC FUNCTION rowsetFieldBinary(self,field,newval)
	LOCAL fieldno :=;
		IIF(VALTYPE(field) == "C", SQLFieldNo(self:rowset,field), field)
RETURN SQLFieldBinary(self:rowset,fieldno,newval)
