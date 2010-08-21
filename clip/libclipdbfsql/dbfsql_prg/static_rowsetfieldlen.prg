STATIC FUNCTION rowsetFieldLen(self,field)
	LOCAL fieldno :=;
		IIF(VALTYPE(field) == "C", SQLFieldNo(self:rowset,field), field)
RETURN SQLFieldLen(self:rowset,fieldno)
