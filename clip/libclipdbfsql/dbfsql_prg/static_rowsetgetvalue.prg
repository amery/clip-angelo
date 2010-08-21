STATIC FUNCTION rowsetGetValue(self,field)
	LOCAL fieldno :=;
		IIF(VALTYPE(field) == "C", SQLFieldNo(self:rowset,field), field)
RETURN self:ToClip(fieldno)
