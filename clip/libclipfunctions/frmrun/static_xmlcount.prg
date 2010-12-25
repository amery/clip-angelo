/***
*
*  XMLCOUNT( <cString>, [<nLineLength>], [<nTabSize>],
*     [<lWrap>] ) --> nLineCount
*
*/
STATIC FUNCTION XMLCOUNT( cString, nLineLength, nTabSize, lWrap )
   // Set defaults if none specified
   nLineLength := IF( nLineLength == NIL, 79, nLineLength )
   nTabSize := IF( nTabSize == NIL, 4, nTabSize )
   lWrap := IF( lWrap == NIL, .T., .F. )

   IF nTabSize >= nLineLength
      nTabSize := nLineLength - 1
   ENDIF
   RETURN( MLCOUNT( TRIM(cString), nLineLength, nTabSize, lWrap ) )
