/***
*
*  XMEMOLINE( <cString>, [<nLineLength>], [<nLineNumber>],
*         [<nTabSize>], [<lWrap>] ) --> cLine
*
*/
STATIC FUNCTION XMEMOLINE( cString, nLineLength, nLineNumber, nTabSize, lWrap )

   // Set defaults if none specified
   nLineLength := IF( nLineLength == NIL, 79, nLineLength )
   nLineNumber := IF( nLineNumber == NIL, 1, nLineNumber )
   nTabSize := IF( nTabSize == NIL, 4, nTabSize )
   lWrap := IF( lWrap == NIL, .T., lWrap )

   IF nTabSize >= nLineLength
      nTabSize := nLineLength - 1
   ENDIF

   RETURN( MEMOLINE( cString, nLineLength, nLineNumber, nTabSize, lWrap ) )
