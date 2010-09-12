STATIC FUNCTION rowsetSetValue(self,field,value)
	LOCAL fieldno :=;
		IIF(VALTYPE(field) == "C", SQLFieldNo(self:rowset,field), field)

	SQLSetValue(self:rowset,fieldno,self:FromClip(fieldno,value,.F.))
	SQLSetHot(self:rowset,.T.)
RETURN
