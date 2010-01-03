STATIC FUNCTION rowsetFieldUnsigned(self,field)
	LOCAL fieldno :=;
		IIF(VALTYPE(field) == "C", SQLFieldNo(self:rowset,field), field)
RETURN SQLFieldUnsigned(self:rowset,fieldno)
