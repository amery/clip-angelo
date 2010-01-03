STATIC FUNCTION rowsetAppend(self,data)
	LOCAL keys := mapkeys(data)
	LOCAL stmt,id,pars,i,j,idhash,sql

	_sqlflush(self)
	IF self:insertSQL != NIL
		id := SQLGenId(self:rowset)
		IF id != NIL .AND. self:idname != NIL
			IF data == NIL
				data := map()
			ENDIF
			idhash := HASHSTR(UPPER(self:idname))
			data[idhash] := id
			AADD(keys,idhash)
		ENDIF
		sql := ParsMacros(self,self:insertSQL,data)
		stmt := SQLPrepare(self:conn,sql)
		pars := ParsArray(self,sql,data)
		IF VALTYPE(self:parameters) == "A"
			FOR i:=1 TO LEN(self:parameters)
				AADD(pars,self:parameters[i])
			NEXT
		ENDIF
		SQLCommand(stmt,pars)
		SQLAppend(self:rowset)
		IF VALTYPE(SQLRowId(self:rowset)) == "N" .AND. SQLRowId(self:rowset) != 0
			id := SQLNewId(self:conn,stmt)
			self:SetValue(SQLRowId(self:rowset),id)
		ENDIF
		SQLFreeSTMT(stmt)
	ELSE
		SQLAppend(self:rowset)
	ENDIF
	FOR I:=1 TO LEN(data)
		FOR J:=1 TO SQLNFields(self:rowset)
			IF !(VALTYPE(SQLRowId(self:rowset)) == "N" .AND. J == SQLRowId(self:rowset))
				IF HASHSTR(UPPER(SQLFieldName(self:rowset,J))) == keys[I]
					self:SetValue(J,data[keys[I]])
					EXIT
				ENDIF
			ENDIF
		NEXT
	NEXT
	SQLAddKeys(self:rowset)
	SQLSetHot(self:rowset,.F.)
RETURN
