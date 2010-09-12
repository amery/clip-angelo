/***********************************************************

    Author   : stas (root@depot.pharm.sumy.ua)
    24.11.01 23:47
*/

STATIC FUNCTION GETFIELD( cField )
  LOCAL nPos, SelectFromField, CurSelect, err, Local5
  CurSelect:= Select()

  IF ( ( nPos:= At( "->", cField ) ) > 0 )
    SelectFromField:= Left( cField, nPos - 1 )
    IF ( Select( SelectFromField ) != CurSelect )
      err:= ErrorNew()
      err:severity:=ES_ERROR
      err:gencode:=EG_SYNTAX
      err:subsystem:="DBCMD"
      err:candefaul:=.T.
      err:operation:=cField
      err:subcode:=1101
      Local5:= eval( errorblock(), err )
      IF ( ValType( Local5 ) != "L" .OR. Local5 )
      ENDIF

      BREAK ( err )
    ENDIF

    cField:= SubStr( cField, nPos + 2 )
  ENDIF

  RETURN ( fieldblock( cField ) )
