STATIC FUNCTION rowsetFieldNullable(self,field)
	LOCAL fieldno :=;
		IIF(VALTYPE(field) == "C", SQLFieldNo(self:rowset,field), field)
RETURN SQLFieldNullable(self:rowset,fieldno)
