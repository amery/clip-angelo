/***
*  __LblLoad( cLblFile ) --> aLabel
*  Load a (.lbl) file into a label array as specified in lbldef.ch
*
*/
FUNCTION __LblLoad( cLblFile )
   LOCAL i, j       := 0                  // Counters
   LOCAL cBuff      := SPACE(BUFFSIZE)    // File buffer
   LOCAL nHandle    := 0                  // File handle
   LOCAL nReadCount := 0                  // Bytes read from file
   LOCAL lStatus    := .F.                // Status
   LOCAL nOffset    := FILEOFFSET         // Offset into file
   LOCAL nFileError := F_OK               // File error
   LOCAL cFieldText := ""                 // Text expression container
   LOCAL err                              // error object

   LOCAL cDefPath          // contents of SET DEFAULT string
   LOCAL aPaths            // array of paths
   LOCAL nPathIndex := 0   // iteration counter

   // Create and initialize default label array
   LOCAL aLabel[ LB_COUNT ]
   aLabel[ LB_REMARK ]  := SPACE(60)      // Label remark
   aLabel[ LB_HEIGHT ]  := 5              // Label height
   aLabel[ LB_WIDTH ]   := 35             // Label width
   aLabel[ LB_LMARGIN ] := 0              // Left margin
   aLabel[ LB_LINES ]   := 1              // Lines between labels
   aLabel[ LB_SPACES ]  := 0              // Spaces between labels
   aLabel[ LB_ACROSS ]  := 1              // Number of labels across
   aLabel[ LB_FIELDS ]  := {}             // Array of label fields

   // Open the label file
   nHandle := FOPEN( cLblFile )

   IF ( ! EMPTY( nFileError := FERROR() ) ) .AND. !( "\" $ cLblFile .OR. ":" $ cLblFile )

      // Search through default path; attempt to open label file
      cDefPath := SET( _SET_DEFAULT )
      cDefPath := STRTRAN( cDefPath, ",", ";" )
      aPaths := ListAsArray( cDefPath, ";" )

      FOR nPathIndex := 1 TO LEN( aPaths )
         nHandle := FOPEN( aPaths[ nPathIndex ] + "\" + cLblFile )
         // if no error is reported, we have our label file
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
      err:filename := cLblFile
      Eval(ErrorBlock(), err)
   ENDIF

   // If we got this far, assume the label file is open and ready to go
   // and so go ahead and read it
   nReadCount := FREAD( nHandle, @cBuff, BUFFSIZE )

   // READ ok?
   IF nReadCount == 0
      nFileError := F_EMPTY             // File is empty
   ELSE
      nFileError := FERROR()            // Check for DOS errors
   ENDIF

   IF nFileError == 0

      // Load label dimension into aLabel
      aLabel[ LB_REMARK ] := SUBSTR(cBuff, REMARKOFFSET, REMARKSIZE)
      aLabel[ LB_HEIGHT ] := BIN2W(SUBSTR(cBuff, HEIGHTOFFSET, HEIGHTSIZE))
      aLabel[ LB_WIDTH  ] := BIN2W(SUBSTR(cBuff, WIDTHOFFSET, WIDTHSIZE))
      aLabel[ LB_LMARGIN] := BIN2W(SUBSTR(cBuff, LMARGINOFFSET, LMARGINSIZE))
      aLabel[ LB_LINES  ] := BIN2W(SUBSTR(cBuff, LINESOFFSET, LINESSIZE))
      aLabel[ LB_SPACES ] := BIN2W(SUBSTR(cBuff, SPACESOFFSET, SPACESSIZE))
      aLabel[ LB_ACROSS ] := BIN2W(SUBSTR(cBuff, ACROSSOFFSET, ACROSSSIZE))

      FOR i := 1 TO aLabel[ LB_HEIGHT ]

         // Get the text of the expression
         cFieldText := TRIM( SUBSTR( cBuff, nOffset, FIELDSIZE ) )
         nOffset += 60

         IF !EMPTY( cFieldText )

            AADD( aLabel[ LB_FIELDS ], {} )

            // Field expression
            AADD( aLabel[ LB_FIELDS, i ], &( "{ || " + cFieldText + "}" ) )

            // Text of field
            AADD( aLabel[ LB_FIELDS, i ], cFieldText )

            // Compression option
            AADD( aLabel[ LB_FIELDS, i ], .T. )

       ELSE

         AADD( aLabel[ LB_FIELDS ], NIL )

         ENDIF

      NEXT

      // Close file
      FCLOSE( nHandle )
      nFileError = FERROR()

   ENDIF
   RETURN( aLabel )
