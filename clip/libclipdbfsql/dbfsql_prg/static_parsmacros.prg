STATIC FUNCTION ParsMacros(self,sql,data)
	LOCAL I,J,K
	LOCAL keys := mapkeys(data)
	LOCAL up := UPPER(sql)
	LOCAL lis := ""
	LOCAL fields := ""
	LOCAL values := ""
	LOCAL poslist,posfields,posvalues,ids,f

	poslist := AT("%LIST",up)
	posfields := AT("%FIELDS",up)
	posvalues := AT("%VALUES",up)

	IF poslist==0 .AND. posfields==0 .AND. posvalues==0
		RETURN sql
	ENDIF

	IF LEN(data)==0
		RETURN NIL
	ENDIF

	ids := SQLRowId(self:rowset)

	FOR I:=1 TO LEN(data)
		FOR J:=1 TO SQLNFields(self:rowset)
			f := .T.
			IF VALTYPE(ids) == "N"
				f := (ids != J)
			ELSE
				FOR k:=1 TO LEN(ids)
					IF ids[k] == J
						f := .F.
						BREAK
					ENDIF
				NEXT
			ENDIF
			IF f
				IF HASHSTR(UPPER(SQLFieldName(self:rowset,J))) == keys[I]
					IF poslist!=0
						lis += SQLFieldName(self:rowset,J)+;
							"=:"+SQLFieldName(self:rowset,J)+","
					ENDIF
					IF posfields!=0
						fields += SQLFieldName(self:rowset,J)+","
					ENDIF
					IF posvalues!=0
						values += ":"+SQLFieldName(self:rowset,J)+","
					ENDIF
					EXIT
				ENDIF
			ENDIF
		NEXT
	NEXT

	IF poslist != 0
		lis := LEFT(lis,LEN(lis)-1)
		WHILE (poslist := AT("%LIST",UPPER(sql))) != 0
			sql := STUFF(sql,poslist,5,lis)
		ENDDO
	ENDIF
	IF posfields != 0
		fields := LEFT(fields,LEN(fields)-1)
		WHILE (posfields := AT("%FIELDS",UPPER(sql))) != 0
			sql := STUFF(sql,posfields,7,fields)
		ENDDO
	ENDIF
	IF posvalues != 0
		values := LEFT(values,LEN(values)-1)
		WHILE (posvalues := AT("%VALUES",UPPER(sql))) != 0
			sql := STUFF(sql,posvalues,7,values)
		ENDDO
	ENDIF

RETURN sql
