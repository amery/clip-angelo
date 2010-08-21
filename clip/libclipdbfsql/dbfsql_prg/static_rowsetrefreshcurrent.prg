STATIC FUNCTION rowsetRefreshCurrent(self,data)
	LOCAL stmt,sql,pars,i,ids

	IF self:refreshSQL != NIL
		IF VALTYPE(data) != "O"
			data := map()
		ENDIF

		ids := SQLRowId(self:rowset)
		IF VALTYPE(ids) == "N"
			data[HASHSTR(UPPER(;
				SQLFieldName(self:rowset,ids)))] := self:GetValue(ids)
		ELSE
			FOR i:=1 TO LEN(ids)
				data[HASHSTR(UPPER(;
					SQLFieldName(self:rowset,ids[i])))] := self:GetValue(ids[i])
			NEXT
		ENDIF
		stmt := SQLPrepare(self:conn,self:refreshSQL)
		pars := ParsArray(self,self:refreshSQL,data)
		IF VALTYPE(self:parameters) == "A"
			FOR i:=1 TO LEN(self:parameters)
				AADD(pars,self:parameters[i])
			NEXT
		ENDIF
		SQLRefresh(self:rowset,stmt,pars)
		SQLFreeSTMT(stmt)
	ENDIF
RETURN
