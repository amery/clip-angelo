STATIC FUNCTION rowsetDelete(self)
	LOCAL ret,pars,stmt,i

	IF SQLLastrec(self:rowset) > 0 .AND. self:deleteSQL != NIL
		stmt := SQLPrepare(self:conn,self:deleteSQL)
		pars := ParsArray(self,self:deleteSQL,self:Read())
		IF VALTYPE(self:parameters) == "A"
			FOR i:=1 TO LEN(self:parameters)
				AADD(pars,self:parameters[i])
			NEXT
		ENDIF
		SQLCommand(stmt,pars)
		SQLFreeSTMT(stmt)
	ENDIF
	ret := SQLDelete(self:rowset)
RETURN ret
