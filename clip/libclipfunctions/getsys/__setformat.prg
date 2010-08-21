PROCEDURE __SetFormat( b )
   oStatus:format := IIF( VALTYPE( b ) == "B", b, NIL )
RETURN
