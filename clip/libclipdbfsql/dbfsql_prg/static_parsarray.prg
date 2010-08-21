STATIC FUNCTION ParsArray(self,sql,data)
	LOCAL ar := ARRAY(0)
	LOCAL iter := sql
	LOCAL b,e
	LOCAL name
	LOCAL I
	LOCAL fieldno
	LOCAL val

	DO WHILE (b := AT(":",iter)) != 0
		iter := SUBSTR(iter,b)
		e := AtAny(" ,;)",iter)
		name := SUBSTR(iter,2,e-2)
		iter := SUBSTR(iter,e+1)
		IF ASCAN(ar,{|x| x[1]==name}) == 0
			fieldno := self:FieldNo(name)
			IF fieldno != 0
				IF data[HASHSTR(UPPER(name))] != NIL
					val := self:FromClip(fieldno,data[HASHSTR(UPPER(name))],.T.)
					AADD(ar,{name,val,SQLFieldTypeSQL(self:rowset,fieldno),;
						SQLFieldBinary(self:rowset,fieldno)})
				ELSE
					AADD(ar,{name,IIF(self:RDBMS=="IB",NIL,"null"),;
						self:FieldTypeSQL(fieldno)})
				ENDIF
			ENDIF
		ENDIF
	ENDDO
RETURN ar
