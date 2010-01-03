/***
*     MakeStr( <exp>, <cType> ) --> value
*     Convert a value of any data type into string to add to the group header
*/
STATIC FUNCTION MakeAStr( uVar, cType )
   LOCAL cString
   DO CASE
   CASE UPPER(cType) == "D"
      cString := DTOC( uVar )

   CASE UPPER(cType) == "L"
      cString := IF( uVar, "T", "F" )

   CASE UPPER(cType) == "N"
      cString := STR( uVar )

   CASE UPPER(cType) == "C"
      cString := uVar

   OTHERWISE
      cString := "INVALID EXPRESSION"
   ENDCASE
   RETURN( cString )
