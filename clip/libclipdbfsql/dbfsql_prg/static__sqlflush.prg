STATIC PROCEDURE _sqlflush(self)
	LOCAL data,sql,stmt,pars,ids,i

	IF SQLGetHot(self:rowset) .AND. self:updateSQL != NIL;
		.AND. !(self:Bof() .AND. self:Eof())

		data := self:Read()
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
		sql := ParsMacros(self,self:updateSQL,data)
		stmt := SQLPrepare(self:conn,sql)
		pars := ParsArray(self,sql,data)
		IF VALTYPE(self:parameters) == "A"
			FOR i:=1 TO LEN(self:parameters)
				AADD(pars,self:parameters[i])
			NEXT
		ENDIF
		SQLCommand(stmt,pars)
		SQLFreeSTMT(stmt)
	ENDIF
	SQLSetHot(self:rowset,.F.)
RETURN
