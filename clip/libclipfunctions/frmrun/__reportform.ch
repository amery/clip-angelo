/***
*
*  __ReportForm( <cFRMName>, [<lPrinter>], <cAltFile>,
*         [<lNoConsole>], <bFor>, <bWhile>, <nNext>, <nRecord>,
*         <lRest>, <lPlain>, [<cHeading>], [<lBEject>],
*         [<lSummary>] )
*
*/
PROCEDURE __ReportForm( cFRMName, lPrinter, cAltFile, lNoConsole, bFor, ;
                       bWhile, nNext, nRecord, lRest, lPlain, cHeading, ;
                       lBEject, lSummary )

   LOCAL lPrintOn, lConsoleOn // Status of PRINTER and CONSOLE
   LOCAL cExtraFile, lExtraState // Status of EXTRA
   LOCAL nCol, nGroup
   LOCAL xBreakVal, lBroke := .F.
   LOCAL err

   LOCAL lAnyTotals
   LOCAL lAnySubTotals

   // Resolve parameters
   IF cFRMName == NIL
      err := ErrorNew()
      err:severity := ES_ERROR
      err:genCode := EG_ARG
      err:subSystem := "FRMLBL"
      Eval(ErrorBlock(), err)
   ELSE
      IF AT( ".", cFRMName ) == 0
         cFRMName := TRIM( cFRMName ) + ".FRM"
      ENDIF
   ENDIF

#ifdef OLDCODE
   IF lPrinter == NIL
     lPrinter   := .F.
   ENDIF
#endif

   IF cHeading == NIL
     cHeading := ""
   ENDIF

   // Set output devices
#ifdef OLDCODE
   lPrintOn   := SET( _SET_PRINTER, lPrinter )

   lConsoleOn := SET( _SET_CONSOLE, .F. )
   SET( _SET_CONSOLE, ! ( lNoConsole .OR. !lConsoleOn ) )
#endif

   lPrintOn   := IF( lPrinter,   SET( _SET_PRINTER, lPrinter ), ;
	                               SET( _SET_PRINTER ) )

	 lConsoleOn := IF( lNoConsole, SET( _SET_CONSOLE, .F.),       ;
                                 SET( _SET_CONSOLE) )

   IF lPrinter                   // To the printer
     lFormFeeds := .T.
   ELSE
     lFormFeeds := .F.
   ENDIF

   IF (!Empty(cAltFile))            // To file
     cExtraFile := SET( _SET_EXTRAFILE, cAltFile )
     lExtraState := SET( _SET_EXTRA, .T. )
   ENDIF


   BEGIN SEQUENCE
     	aReportData := __FrmLoad( cFRMName )  // Load the frm into an array
     	nMaxLinesAvail := aReportData[RP_LINES]

     	// Modify aReportData based on the report parameters
#ifdef OLDCODE
     	IF lSummary != NIL             // Set the summary only flag
#else
      IF lSummary == .T.             // Set the summary only flag
#endif
          aReportData[ RP_SUMMARY ] := lSummary
     	ENDIF
     	IF lBEject != NIL .AND. lBEject
         	aReportData[ RP_BEJECT ]  := .F.
     	ENDIF
     	IF lPlain                      // Set plain report flag
       		aReportData[ RP_PLAIN ]   := .T.
       		cHeading               := ""
       		lFormFeeds             := .F.
     	ENDIF
     	aReportData[ RP_HEADING ]    := cHeading

     	// Add to the left margin if a SET MARGIN has been defined
     	// NOTE: uncommenting this line will cause REPORT FORM to respect
     	// SET MARGIN to screen/to file, but double the margin TO PRINT
     	// aReportData[ RP_LMARGIN ] += SET( _SET_MARGIN )

     	nPageNumber := 1                  // Set the initial page number
     	lFirstPass  := .T.             // Set the first pass flag

     	nLinesLeft  := aReportData[ RP_LINES ]

#ifdef S87_COMPAT
		QOUT()		// output additional line on first page
		nLinesLeft--
#endif

     	// Check to see if a "before report" eject, or TO FILE has been specified
     	IF aReportData[ RP_BEJECT ]
	      EjectPage()

     	ENDIF

     	// Generate the initial report header manually (in case there are no
     	// records that match the report scope)
     	ReportHeader()

     	// Initialize aReportTotals to track both group and report totals, then
     	// set the column total elements to 0 if they are to be totaled, otherwise
     	// leave them NIL
     	aReportTotals := ARRAY( LEN(aReportData[RP_GROUPS]) + 1, ;
                       	LEN(aReportData[RP_COLUMNS]) )

     	// Column total elements
     	FOR nCol := 1 TO LEN(aReportData[RP_COLUMNS])
       	IF aReportData[RP_COLUMNS,nCol,RC_TOTAL]
         	FOR nGroup := 1 TO LEN(aReportTotals)
            	aReportTotals[nGroup,nCol] := 0
         	NEXT
       	ENDIF
     	NEXT

     	// Initialize aGroupTotals as an array
     	aGroupTotals := ARRAY( LEN(aReportData[RP_GROUPS]) )

     	// Execute the actual report based on matching records
     	DBEval( { || ExecuteReport() }, bFor, bWhile, nNext, nRecord, lRest )

     	// Generate any totals that may have been identified
     	// Make a pass through all the groups
     	FOR nGroup := LEN(aReportData[RP_GROUPS]) TO 1 STEP -1


       	// make sure group has subtotals
       	lAnySubTotals := .F.
       	FOR nCol := 1 TO LEN(aReportData[RP_COLUMNS])
         	IF aReportData[RP_COLUMNS,nCol,RC_TOTAL]
            	lAnySubTotals := .T.
            	EXIT              // NOTE
         	ENDIF
       	NEXT

       	IF !lAnySubTotals
         	LOOP                 // NOTE
       	ENDIF


       	// Check to see if we need to eject the page
       	IF nLinesLeft < 2
         	EjectPage()
         	IF aReportData[ RP_PLAIN ]
            	nLinesLeft := 1000
         	ELSE
            	ReportHeader()
         	ENDIF
       	ENDIF

       	// Print the first line
       	PrintIt( SPACE(aReportData[RP_LMARGIN]) + ;
            	IF(nGroup==1,NationMsg(_RF_SUBTOTAL),;
                            NationMsg(_RF_SUBSUBTOTAL) ) )

       	// Print the second line
       	QQOUT( SPACE(aReportData[RP_LMARGIN]) )
       	FOR nCol := 1 TO LEN(aReportData[RP_COLUMNS])
         	IF nCol > 1
            	QQOUT( " " )
         	ENDIF
         	IF aReportData[RP_COLUMNS,nCol,RC_TOTAL]
            	QQOUT( TRANSFORM(aReportTotals[nGroup+1,nCol], ;
              	aReportData[RP_COLUMNS,nCol,RC_PICT]) )
         	ELSE
            	QQOUT( SPACE(aReportData[RP_COLUMNS,nCol,RC_WIDTH]) )
         	ENDIF
       	NEXT

       	// Send a cr/lf for the last line
       	QOUT()

     	NEXT

#ifdef OLDCODE
     	// Generate the "Grand totals"
     	// Check to see if we need to eject the page
     	IF nLinesLeft < 2
       	EjectPage()
       	IF aReportData[ RP_PLAIN ]
         	nLinesLeft := 1000
       	ELSE
         	ReportHeader()
       	ENDIF
     	ENDIF
#endif

     	// Any report totals?
     	lAnyTotals := .F.
     	FOR nCol := 1 TO LEN(aReportData[RP_COLUMNS])
       	IF aReportData[RP_COLUMNS,nCol,RC_TOTAL]
         	lAnyTotals := .T.
         	EXIT
       	ENDIF
     	NEXT nCol


     	IF lAnyTotals

#ifndef OLDCODE
        // Check to see if we need to eject the page
        IF nLinesLeft < 2
          EjectPage()
          IF aReportData[ RP_PLAIN ]
            nLinesLeft := 1000
          ELSE
            ReportHeader()
          ENDIF
        ENDIF
#endif

        	// Print the first line
        	PrintIt( SPACE(aReportData[RP_LMARGIN]) + NationMsg(_RF_TOTAL ) )

        	// Print the second line
        	QQOUT( SPACE(aReportData[RP_LMARGIN]) )
        	FOR nCol := 1 TO LEN(aReportData[RP_COLUMNS])
          	IF nCol > 1
            	QQOUT( " " )
          	ENDIF
          	IF aReportData[RP_COLUMNS,nCol,RC_TOTAL]
            	QQOUT( TRANSFORM(aReportTotals[1,nCol], ;
               	aReportData[RP_COLUMNS,nCol,RC_PICT]) )
          	ELSE
            	QQOUT( SPACE(aReportData[RP_COLUMNS,nCol,RC_WIDTH]) )
          	ENDIF
        	NEXT nCol

        	// Send a cr/lf for the last line
        	QOUT()

     	ENDIF

     	// Check to see if an "after report" eject, or TO FILE has been specified
     	IF aReportData[ RP_AEJECT ]
      	EjectPage()
     	ENDIF


   RECOVER USING xBreakVal

		lBroke := .T.

   END SEQUENCE


   // Clean up and leave
   aReportData   := NIL          // Recover the space
   aReportTotals  := NIL
   aGroupTotals   := NIL
   nPageNumber   := NIL
   lFirstPass    := NIL
   nLinesLeft    := NIL
   lFormFeeds    := NIL
   nMaxLinesAvail := NIL

   // clean up
   SET( _SET_PRINTER, lPrintOn )    // Set the printer back to prior state
   SET( _SET_CONSOLE, lConsoleOn )     // Set the console back to prior state

   IF (!Empty(cAltFile))            // Set extrafile back
     SET( _SET_EXTRAFILE, cExtraFile )
     SET( _SET_EXTRA, lExtraState )
   ENDIF

   IF lBroke
     // keep the break value going
     BREAK xBreakVal
   END

   RETURN
