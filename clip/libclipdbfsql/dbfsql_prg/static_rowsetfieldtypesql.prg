STATIC FUNCTION rowsetFieldTypeSQL(self,field)
	LOCAL fieldno :=;
		IIF(VALTYPE(field) == "C", SQLFieldNo(self:rowset,field), field)
RETURN SQLFieldTypeSQL(self:rowset,fieldno)
