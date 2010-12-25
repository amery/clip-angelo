FUNCTION _FromClipToRDBMS(rowset,conn,rdbms,fieldno,value,totext)
	LOCAL ret
	LOCAL lob

	IF rdbms == "PG"
		ret := CLIP("CLIP2PG",rowset,fieldno,value,totext)
		IF totext
			ret := "'"+ret+"'"
		ENDIF
	ELSEIF rdbms == "MS"
		ret := CLIP("CLIP2MS",rowset,fieldno,value,totext)
		IF totext
			ret := "'"+ret+"'"
		ENDIF
	ELSEIF rdbms == "OR"
		lob := NIL
		IF totext
			lob := CLIP("OR_GETLOBLOC",rowset,fieldno)
		ENDIF
		ret := CLIP("CLIP2OR",rowset,fieldno,value,totext)
		IF lob!=NIL
			ret := lob+ret
		ENDIF
	ELSEIF rdbms == "ODBC"
		ret := CLIP("CLIP2ODBC",rowset,fieldno,value,totext)
		IF totext
			ret := "'"+ret+"'"
		ENDIF
	ELSEIF rdbms == "IB"
		ret := CLIP("CLIP2IB",rowset,fieldno,value,totext)
	ELSEIF rdbms == "DBTCP"
		ret := CLIP("CLIP2DBTCP",rowset,fieldno,value,totext)
		IF totext .AND. VALTYPE(value) != 'N'
			ret := "'"+ret+"'"
		ENDIF
	ENDIF
	IF VALTYPE(ret) == "C" .AND. !SQLFieldBinary(rowset,fieldno)
		SQLLocWrite(conn,@ret)
	ENDIF
	SQLSetHot(rowset,.T.)
RETURN ret
