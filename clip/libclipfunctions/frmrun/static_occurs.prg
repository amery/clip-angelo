/***
*  Occurs( <cSearch>, <cTarget> ) --> nCount
*  Determine the number of times <cSearch> is found in <cTarget>
*
*/
STATIC FUNCTION Occurs( cSearch, cTarget )
   LOCAL nPos, nCount := 0
   DO WHILE !EMPTY( cTarget )
      IF (nPos := AT( cSearch, cTarget )) != 0
         nCount++
         cTarget := SUBSTR( cTarget, nPos + 1 )
      ELSE
         // End of string
         cTarget := ""
      ENDIF
   ENDDO
   RETURN nCount
