FUNCTION _FromRDBMSToClip(rowset,conn,RDBMS,fieldno)
	LOCAL ret

	IF RDBMS == "PG"
		ret := CLIP("PG2CLIP",rowset,fieldno)
	ELSEIF RDBMS == "MS"
		ret := CLIP("MS2CLIP",rowset,fieldno)
	ELSEIF RDBMS == "OR"
		ret := CLIP("OR2CLIP",rowset,fieldno)
	ELSEIF RDBMS == "ODBC"
		ret := CLIP("ODBC2CLIP",rowset,fieldno)
	ELSEIF RDBMS == "IB"
		ret := CLIP("IB2CLIP",rowset,fieldno)
	ELSEIF RDBMS == "DBTCP"
		ret := CLIP("DBTCP2CLIP",rowset,fieldno)
	ENDIF
	IF VALTYPE(ret) == "C" .AND. !SQLFieldBinary(rowset,fieldno)
		SQLLocRead(conn,@ret)
	ENDIF
RETURN ret
