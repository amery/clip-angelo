/***
*
*  __FrmLoad( cFrmFile ) --> aReport
*  Reads a report (.frm) file and creates a report array
*
*  Notes:
*
*      1.   Report file name has extension.
*      2.   File error number placed in nFileError
*      3.   Offsets start at 1. Offsets are into a Clipper string, 1 to 1990
*      4.   The offsets mentioned in these notes are actual DOS FILE offsets,
*           not like the offsets declared in the body of FrmLoad()
*           which are Clipper STRING offsets.
*      5.   Report file length is 7C6h (1990d) bytes.
*      6.   Expression length array starts at 04h (4d) and can
*           contain upto 55 short (2 byte) numbers.
*      7.   Expression offset index array starts at 72h (114d) and
*           can contain upto 55 short (2 byte) numbers.
*      8.   Expression area starts at offset E0h (224d).
*      9.   Expression area length is 5A0h (1440d).
*     10.   Expressions in expression area are null terminated.
*     11.   Field expression area starts at offset 680h (1664d).
*     12.   Field expressions (column definition) are null terminated.
*     13.   Field expression area can contain upto 25 12-byte blocks.
*/

/***
*
*  __FrmLoad( <cFrmFile> ) --> aReport
*
*/
FUNCTION __FrmLoad( cFrmFile )
   LOCAL cFieldsBuff
   LOCAL cParamsBuff
   LOCAL nFieldOffset   := 0
   LOCAL cFileBuff      := SPACE(SIZE_FILE_BUFF)
   LOCAL cGroupExp      := SPACE(200)
   LOCAL cSubGroupExp   := SPACE(200)
   LOCAL nColCount      := 0        // Number of columns in report
   LOCAL nCount
   LOCAL nFrmHandle                 // (.frm) file handle
   LOCAL nBytesRead                 // Read/write and content record counter
   LOCAL nPointer       := 0        // Points to an offset into EXPR_BUFF string
   LOCAL nFileError                 // Contains current file error
   LOCAL cOptionByte                // Contains option byte

   LOCAL aReport[ RP_COUNT ]        // Create report array
   LOCAL err                        // error object

   LOCAL cDefPath          // contents of SET DEFAULT string
   LOCAL aPaths            // array of paths
   LOCAL nPathIndex := 0   // iteration counter

   LOCAL s, paths
   LOCAL i
	LOCAL aHeader				// temporary storage for report form headings
	LOCAL nHeaderIndex		// index into temporary header array

   // Initialize STATIC buffer values
   cLengthsBuff  := ""
   cOffsetsBuff  := ""
   cExprBuff     := ""

   // Default report values
   aReport[ RP_HEADER ]    := {}
   aReport[ RP_WIDTH ]     := 80
   aReport[ RP_LMARGIN ]   := 8
   aReport[ RP_RMARGIN ]   := 0
   aReport[ RP_LINES ]     := 58
   aReport[ RP_SPACING ]   := 1
   aReport[ RP_BEJECT ]    := .T.
   aReport[ RP_AEJECT ]    := .F.
   aReport[ RP_PLAIN ]     := .F.
   aReport[ RP_SUMMARY ]   := .F.
   aReport[ RP_COLUMNS ]   := {}
   aReport[ RP_GROUPS ]    := {}
   aReport[ RP_HEADING ]   := ""

   // Open the report file
   nFrmHandle := FOPEN( cFrmFile )

   IF ( !EMPTY( nFileError := FERROR() ) ) .AND. !( "\" $ cFrmFile .OR. ":" $ cFrmFile )

      // Search through default path; attempt to open report file
      cDefPath := SET( _SET_DEFAULT ) + ";" + SET( _SET_PATH )
      cDefPath := STRTRAN( cDefPath, ",", ";" )
      aPaths := ListAsArray( cDefPath, ";" )

      FOR nPathIndex := 1 TO LEN( aPaths )
         nFrmHandle := FOPEN( aPaths[ nPathIndex ] + "\" + cFrmFile )
         // if no error is reported, we have our report file
         IF EMPTY( nFileError := FERROR() )
            EXIT

         ENDIF

      NEXT nPathIndex

   ENDIF

   // File error
   IF nFileError != F_OK
      err := ErrorNew()
      err:severity := ES_ERROR
      err:genCode := EG_OPEN
      err:subSystem := "FRMLBL"
      err:osCode := nFileError
      err:filename := cFrmFile
      Eval(ErrorBlock(), err)
   ENDIF

   // OPEN ok?
   IF nFileError = F_OK

      // Go to START of report file
      FSEEK(nFrmHandle, 0)

      // SEEK ok?
      nFileError = FERROR()
      IF nFileError = F_OK

         // Read entire file into process buffer
         nBytesRead = FREAD(nFrmHandle, @cFileBuff, SIZE_FILE_BUFF)

         // READ ok?
         IF nBytesRead = 0
            nFileError = F_EMPTY        // file is empty
         ELSE
            nFileError = FERROR()       // check for DOS errors
         ENDIF

         IF nFileError = F_OK

            // Is this a .FRM type file (2 at start and end of file)
            IF BIN2W(SUBSTR(cFileBuff, 1, 2)) = 2 .AND.;
              BIN2W(SUBSTR(cFileBuff, SIZE_FILE_BUFF - 1, 2)) = 2

               nFileError = F_OK
            ELSE
               nFileError = F_ERROR
            ENDIF

         ENDIF

      ENDIF

      // Close file
      IF !FCLOSE(nFrmHandle)
         nFileError = FERROR()
      ENDIF

   ENDIF

// File existed, was opened and read ok and is a .FRM file
IF nFileError = F_OK

   // Fill processing buffers
   cLengthsBuff = SUBSTR(cFileBuff, LENGTHS_OFFSET, SIZE_LENGTHS_BUFF)
   cOffsetsBuff = SUBSTR(cFileBuff, OFFSETS_OFFSET, SIZE_OFFSETS_BUFF)
   cExprBuff    = SUBSTR(cFileBuff, EXPR_OFFSET, SIZE_EXPR_BUFF)
   cFieldsBuff  = SUBSTR(cFileBuff, FIELDS_OFFSET, SIZE_FIELDS_BUFF)
   cParamsBuff  = SUBSTR(cFileBuff, PARAMS_OFFSET, SIZE_PARAMS_BUFF)


   // Process report attributes
   // Report width
   aReport[ RP_WIDTH ]   := BIN2W(SUBSTR(cParamsBuff, PAGE_WIDTH_OFFSET, 2))

   // Lines per page
   aReport[ RP_LINES ]   := BIN2W(SUBSTR(cParamsBuff, LNS_PER_PAGE_OFFSET, 2))

   // Page offset (left margin)
   aReport[ RP_LMARGIN ] := BIN2W(SUBSTR(cParamsBuff, LEFT_MRGN_OFFSET, 2))

   // Page right margin (not used)
   aReport[ RP_RMARGIN ] := BIN2W(SUBSTR(cParamsBuff, RIGHT_MGRN_OFFSET, 2))

   nColCount  = BIN2W(SUBSTR(cParamsBuff, COL_COUNT_OFFSET, 2))

   // Line spacing
   // Spacing is 1, 2, or 3
   aReport[ RP_SPACING ] := IF( IsAffirm(SUBSTR(cParamsBuff, ;
	 DBL_SPACE_OFFSET, 1)) , 2, 1)

   // Summary report flag
   aReport[ RP_SUMMARY ] := IF( IsAffirm(SUBSTR(cParamsBuff, ;
    SUMMARY_RPT_OFFSET, 1)) , .T., .F.)

   // Process report eject and plain attributes option byte
   cOptionByte = ASC(SUBSTR(cParamsBuff, OPTION_OFFSET, 1))

   IF INT(cOptionByte / 4) = 1
      aReport[ RP_PLAIN ] := .T.          // Plain page
      cOptionByte -= 4
   ENDIF

   IF INT(cOptionByte / 2) = 1
      aReport[ RP_AEJECT ] := .T.         // Page eject after report
      cOptionByte -= 2
   ENDIF

   IF INT(cOptionByte / 1) = 1
      aReport[ RP_BEJECT ] := .F.         // Page eject before report
      cOptionByte -= 1
   ENDIF

   // Page heading, report title
   nPointer = BIN2W(SUBSTR(cParamsBuff, PAGE_HDR_OFFSET, 2))

	// Retrieve the header stored in the .FRM file
	nHeaderIndex := 4
   aHeader := ParseHeader( GetExpr( nPointer ), nHeaderIndex )

	// certain that we have retrieved all heading entries from the .FRM file, we
	// now retract the empty headings
	DO WHILE ( nHeaderIndex > 0 )
		IF ! EMPTY( aHeader[ nHeaderIndex ] )
			EXIT
		ENDIF
		nHeaderIndex--
	ENDDO

	aReport[ RP_HEADER ] := IIF( EMPTY( nHeaderIndex ) , {}, ;
		ASIZE( aHeader, nHeaderIndex ) )

   // Process Groups
   // Group
   nPointer = BIN2W(SUBSTR(cParamsBuff, GRP_EXPR_OFFSET, 2))

   IF !EMPTY(cGroupExp := GetExpr( nPointer ))

      // Add a new group array
      AADD( aReport[ RP_GROUPS ], ARRAY( RG_COUNT ))

      // Group expression
      aReport[ RP_GROUPS ][1][ RG_TEXT ] := cGroupExp
      aReport[ RP_GROUPS ][1][ RG_EXP ] := &( "{ || " + cGroupExp + "}" )
      IF USED()
         aReport[ RP_GROUPS ][1][ RG_TYPE ] := ;
                        VALTYPE( EVAL( aReport[ RP_GROUPS ][1][ RG_EXP ] ) )
      ENDIF

      // Group header
      nPointer = BIN2W(SUBSTR(cParamsBuff, GRP_HDR_OFFSET, 2))
      aReport[ RP_GROUPS ][1][ RG_HEADER ] := GetExpr( nPointer )

      // Page eject after group
      aReport[ RP_GROUPS ][1][ RG_AEJECT ] := IF(IsAffirm(SUBSTR(cParamsBuff, ;
      PE_OFFSET, 1)), .T., .F.)

   ENDIF

   // Subgroup
   nPointer = BIN2W(SUBSTR(cParamsBuff, SUB_EXPR_OFFSET, 2))

   IF !EMPTY(cSubGroupExp := GetExpr( nPointer ))

      // Add new group array
      AADD( aReport[ RP_GROUPS ], ARRAY( RG_COUNT ))

      // Subgroup expression
      aReport[ RP_GROUPS ][2][ RG_TEXT ] := cSubGroupExp
      aReport[ RP_GROUPS ][2][ RG_EXP ] := &( "{ || " + cSubGroupExp + "}" )
      IF USED()
         aReport[ RP_GROUPS ][2][ RG_TYPE ] := ;
                        VALTYPE( EVAL( aReport[ RP_GROUPS ][2][ RG_EXP ] ) )
      ENDIF

      // Subgroup header
      nPointer = BIN2W(SUBSTR(cParamsBuff, SUB_HDR_OFFSET, 2))
      aReport[ RP_GROUPS ][2][ RG_HEADER ] := GetExpr( nPointer )

      // Page eject after subgroup
      aReport[ RP_GROUPS ][2][ RG_AEJECT ] := .F.

   ENDIF

   // Process columns
   nFieldOffset := 12      // dBASE skips first 12 byte fields block.
   FOR nCount := 1 to nColCount

      AADD( aReport[ RP_COLUMNS ], GetColumn( cFieldsBuff, @nFieldOffset ) )

   NEXT nCount

ENDIF

RETURN aReport
