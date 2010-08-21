/***
*  GetExpr( nPointer ) --> cString
*
*  Reads an expression from EXPR_BUFF via the OFFSETS_BUFF and returns
*  a pointer to offset contained in OFFSETS_BUFF that in turn points
*  to an expression located in the EXPR_BUFF string.
*
*  Notes:
*
*     1. The expression is empty if:
*         a. Passed pointer is equal to 65535
*         b. Character following character pointed to by pointer is CHR(0)
*
*/
STATIC FUNCTION GetExpr( nPointer )
   LOCAL nExprOffset   := 0
   LOCAL nExprLength   := 0
   LOCAL nOffsetOffset := 0
   LOCAL cString := ""

   // Stuff for dBASE compatability.
   IF nPointer != 65535

      // Convert DOS FILE offset to CLIPPER string offset
      nPointer++

      // Calculate offset into OFFSETS_BUFF
      IF nPointer > 1
         nOffsetOffset = (nPointer * 2) - 1
      ENDIF

      nExprOffset = BIN2W(SUBSTR(cOffsetsBuff, nOffsetOffset, 2))
      nExprLength = BIN2W(SUBSTR(cLengthsBuff, nOffsetOffset, 2))

      // EXPR_OFFSET points to a NULL, so add one (+1) to get the string
      // and subtract one (-1) from EXPR_LENGTH for correct length

      nExprOffset++
      nExprLength--

      // Extract string
      cString = SUBSTR(cExprBuff, nExprOffset, nExprLength)

      // dBASE does this so we must do it too
      // Character following character pointed to by pointer is NULL
      IF CHR(0) == SUBSTR(cString, 1, 1) .AND. LEN(SUBSTR(cString,1,1)) = 1
         cString = ""
      ENDIF
   ENDIF

   RETURN (cString)
