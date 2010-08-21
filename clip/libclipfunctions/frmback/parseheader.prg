/***
*
*  ParseHeader( <cHeaderString>, <nFields> ) --> aPageHeader
*
*	Parse report header (title) field from .FRM and populate page header
*	array. Processing is complicated somewhat by varying .FRM storage
*  formats of dBASE III+ and CA-Clipper. Although similar to ListAsArray(),
*	this function also accounts for fixed-length strings.
*
*/
FUNCTION ParseHeader( cHeaderString, nFields )
	LOCAL cItem
	LOCAL nItemCount := 0
	LOCAL aPageHeader := {}
   LOCAL nHeaderLen := 254
	LOCAL nPos

	DO WHILE ( ++nItemCount <= nFields )

		cItem := SUBSTR( cHeaderString, 1, nHeaderLen )

		// check for explicit delimiter
		nPos := AT( ";", cItem )

		IF ! EMPTY( nPos )
			// delimiter present
			AADD( aPageHeader, SUBSTR( cItem, 1, nPos - 1 ) )
		ELSE
			IF EMPTY( cItem )
				// empty string for S87 and 5.0 compatibility
				AADD( aPageHeader, "" )
			ELSE
				// exception
				AADD( aPageHeader, cItem )

			ENDIF
			// empty or not, we jump past the field
			nPos := nHeaderLen
		ENDIF

		cHeaderString := SUBSTR( cHeaderString, nPos + 1 )

	ENDDO

	RETURN( aPageHeader )
