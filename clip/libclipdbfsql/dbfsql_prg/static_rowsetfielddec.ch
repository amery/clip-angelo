STATIC FUNCTION rowsetFieldDec(self,field)
	LOCAL fieldno :=;
		IIF(VALTYPE(field) == "C", SQLFieldNo(self:rowset,field), field)
RETURN SQLFieldDec(self:rowset,fieldno)
