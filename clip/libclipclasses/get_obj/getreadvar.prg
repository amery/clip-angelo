FUNCTION GetReadVar( oGet )

   LOCAL cName := UPPER( oGet:name )
   LOCAL i

   IF !( oGet:subscript == NIL )
      FOR i := 1 TO LEN( oGet:subscript )
	 cName += "[" + LTRIM( STR( oGet:subscript[i] ) ) + "]"
      NEXT
   ENDIF
RETURN ( cName )
