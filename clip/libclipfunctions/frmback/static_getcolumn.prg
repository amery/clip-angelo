/***
*  GetColumn( <cFieldBuffer>, @<nOffset> ) --> aColumn
*
*  Get a COLUMN element from FIELDS_BUFF string using nOffset to point to
*  the current FIELDS_OFFSET block.
*
*  Notes:
*     1. The Header or Contents expressions are empty if:
*        a. Passed pointer is equal to 65535
*        b. Character following character pointed to by pointer is CHR(0)
*
*/
STATIC FUNCTION GetColumn( cFieldsBuffer, nOffset )
   LOCAL nPointer := 0, nNumber := 0, aColumn[ RC_COUNT ], cType
   local s

   // Column width
   aColumn[ RC_WIDTH ] := BIN2W(SUBSTR(cFieldsBuffer, nOffset + ;
		FIELD_WIDTH_OFFSET, 2))

   // Total column?
   aColumn[ RC_TOTAL ] := IF(IsAffirm(SUBSTR(cFieldsBuffer, nOffset + ;
    FIELD_TOTALS_OFFSET, 1)), .T., .F.)

   // Decimals width
   aColumn[ RC_DECIMALS ] := BIN2W(SUBSTR(cFieldsBuffer, nOffset + ;
		FIELD_DECIMALS_OFFSET, 2))

   // Offset (relative to FIELDS_OFFSET), 'point' to
   // expression area via array OFFSETS[]

   // Content expression
   nPointer = BIN2W(SUBSTR(cFieldsBuffer, nOffset +;
               FIELD_CONTENT_EXPR_OFFSET, 2))
   s:=GetExpr(nPointer)
   aColumn[ RC_TEXT ] := s //GetExpr( nPointer )
   if empty(s)
   	aColumn[ RC_EXP ] := {|| }
   else
   	aColumn[ RC_EXP ] := &( "{ || " + GetExpr( nPointer ) + "}" )
   endif

   // Header expression
   nPointer = BIN2W(SUBSTR(cFieldsBuffer, nOffset +;
               FIELD_HEADER_EXPR_OFFSET, 2))

   aColumn[ RC_HEADER ] := ListAsArray(GetExpr( nPointer ), ";")

   // Column picture
   // Setup picture only if a database file is open
   IF USED()
      cType := VALTYPE( EVAL(aColumn[ RC_EXP ]) )
      aColumn[ RC_TYPE ] := cType
      DO CASE
      CASE cType = "C" .OR. cType = "M"
         aColumn[ RC_PICT ] := REPLICATE("X", aColumn[ RC_WIDTH ])
      CASE cType = "D"
         aColumn[ RC_PICT ] := "@D"
      CASE cType = "N"
         IF aColumn[ RC_DECIMALS ] != 0
            aColumn[ RC_PICT ] := REPLICATE("9", aColumn[ RC_WIDTH ] - aColumn[ RC_DECIMALS ] -1) + "." + ;
                                  REPLICATE("9", aColumn[ RC_DECIMALS ])
         ELSE
            aColumn[ RC_PICT ] := REPLICATE("9", aColumn[ RC_WIDTH ])
         ENDIF
      CASE cType = "L"
         aColumn[ RC_PICT ] := "@L" + REPLICATE("X",aColumn[ RC_WIDTH ]-1)
      ENDCASE
   ENDIF

   // Update offset into ?_buffer
   nOffset += 12

   RETURN ( aColumn )
