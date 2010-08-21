STATIC FUNCTION rowsetWrite(self,data)
	LOCAL datakeys := mapkeys(data)
	LOCAL stmt,sql,pars,ids,I,J

	IF SQLLastrec(self:rowset) > 0
		IF self:updateSQL != NIL
			ids := SQLRowId(self:rowset)
			IF VALTYPE(ids) == "N"
				data[HASHSTR(UPPER(;
					SQLFieldName(self:rowset,ids)))] := self:GetValue(ids)
			ELSE
				FOR i:=1 TO LEN(ids)
					data[HASHSTR(UPPER(;
						SQLFieldName(self:rowset,ids[i])))] :=;
							self:GetValue(ids[i])
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
		FOR I:=1 TO LEN(datakeys)
			FOR J:=1 TO SQLNFields(self:rowset)
				IF HASHSTR(UPPER(SQLFieldName(self:rowset,J))) == datakeys[I]
					self:SetValue(J,data[datakeys[I]])
					EXIT
				ENDIF
			NEXT
		NEXT
	ENDIF
	SQLSetHot(self:rowset,.F.)
RETURN
