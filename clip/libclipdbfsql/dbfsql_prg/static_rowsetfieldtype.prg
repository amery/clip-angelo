STATIC FUNCTION rowsetFieldType(self,field)
	LOCAL fieldno :=;
		IIF(VALTYPE(field) == "C", SQLFieldNo(self:rowset,field), field)
RETURN SQLFieldType(self:rowset,fieldno)
