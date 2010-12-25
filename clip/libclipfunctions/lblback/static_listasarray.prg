/***
*
*  ListAsArray( <cList>, <cDelimiter> ) --> aList
*  Convert a delimited string to an array
*
*/
STATIC FUNCTION ListAsArray( cList, cDelimiter )

   LOCAL nPos
   LOCAL aList := {}                  // Define an empty array
   LOCAL lDelimLast := .F.

   IF cDelimiter == NIL
      cDelimiter := ","
   ENDIF

   DO WHILE ( LEN(cList) <> 0 )

      nPos := AT(cDelimiter, cList)

      IF ( nPos == 0 )
         nPos := LEN(cList)
      ENDIF

      IF ( SUBSTR( cList, nPos, 1 ) == cDelimiter )
         lDelimLast := .T.
         AADD(aList, SUBSTR(cList, 1, nPos - 1)) // Add a new element
      ELSE
         lDelimLast := .F.
         AADD(aList, SUBSTR(cList, 1, nPos)) // Add a new element
      ENDIF

      cList := SUBSTR(cList, nPos + 1)

   ENDDO

   IF ( lDelimLast )
      AADD(aList, "")
   ENDIF

   RETURN aList                       // Return the array

