/***
*  ExecuteReport()
*  Executed by DBEVAL() for each record that matches record scope
*/
STATIC PROCEDURE ExecuteReport
   LOCAL aRecordHeader  := {}          // Header for the current record
   LOCAL aRecordToPrint := {}          // Current record to print
   LOCAL nCol                          // Counter for the column work
   LOCAL nGroup                        // Counter for the group work
   LOCAL lGroupChanged  := .F.         // Has any group changed?
   LOCAL lEjectGrp := .F.              // Group eject indicator
   LOCAL nMaxLines                     // Number of lines needed by record
   LOCAL nLine                         // Counter for each record line
   LOCAL cLine                         // Current line of text for parsing
   LOCAL nLastElement                  // Last element pointer if record is

   LOCAL lAnySubTotals

   // Add to the main column totals
   FOR nCol := 1 TO LEN(aReportData[RP_COLUMNS])
      IF aReportData[RP_COLUMNS,nCol,RC_TOTAL]
         // If this column should be totaled, do it
         aReportTotals[ 1 ,nCol] += ;
                  EVAL( aReportData[RP_COLUMNS,nCol,RC_EXP] )
      ENDIF
   NEXT

   // Determine if any of the groups have changed.  If so, add the appropriate
   // line to aRecordHeader for totaling out the previous records
   IF !lFirstPass                       // Don't bother first time through

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

#ifndef OLDCODE
       // retrieve group eject state from report form
       IF ( nGroup == 1 )
         lEjectGrp := aReportData[ RP_GROUPS, nGroup, RG_AEJECT ]
       ENDIF
#endif

       IF !lAnySubTotals
         LOOP                 // NOTE
       ENDIF

         //  For subgroup processing: check if group has been changed
			   IF MakeAStr(EVAL(aReportData[RP_GROUPS, 1, RG_EXP]),;
                 aReportData[RP_GROUPS, 1, RG_TYPE]) != aGroupTotals[1]
					  lGroupChanged  := .T.
			   ENDIF

         //  If this (sub)group has changed since the last record
         IF lGroupChanged .OR. MakeAStr(EVAL(aReportData[RP_GROUPS,nGroup,RG_EXP]),;
             aReportData[RP_GROUPS,nGroup,RG_TYPE]) != aGroupTotals[nGroup]

            AADD( aRecordHeader, IF(nGroup==1,NationMsg(_RF_SUBTOTAL),;
                                              NationMsg(_RF_SUBSUBTOTAL)) )
            AADD( aRecordHeader, "" )

#ifdef OLDCODE
				// retrieve group eject state from report form
				IF ( nGroup == 1 )
					lEjectGrp := aReportData[ RP_GROUPS, nGroup, RG_AEJECT ]
				ENDIF
#endif

            // Cycle through the columns, adding either the group
            // amount from aReportTotals or spaces wide enough for
            // the non-totaled columns
            FOR nCol := 1 TO LEN(aReportData[RP_COLUMNS])
               IF aReportData[RP_COLUMNS,nCol,RC_TOTAL]
                  aRecordHeader[ LEN(aRecordHeader) ] += ;
                     TRANSFORM(aReportTotals[nGroup+1,nCol], ;
                     aReportData[RP_COLUMNS,nCol,RC_PICT])
                  // Zero out the group totals column from aReportTotals
                  aReportTotals[nGroup+1,nCol] := 0
               ELSE
                  aRecordHeader[ LEN(aRecordHeader) ] += ;
                        SPACE(aReportData[RP_COLUMNS,nCol,RC_WIDTH])
               ENDIF
               aRecordHeader[ LEN(aRecordHeader) ] += " "
            NEXT
            // Get rid of the extra space from the last column
            aRecordHeader[LEN(aRecordHeader)] := ;
                  LEFT( aRecordHeader[LEN(aRecordHeader)], ;
                  LEN(aRecordHeader[LEN(aRecordHeader)]) - 1 )
         ENDIF
      NEXT

   ENDIF

#ifdef OLDCODE
   lFirstPass = .F.
#endif

	IF ( LEN( aRecordHeader ) > 0 ) .AND. lEjectGrp .AND. lGroupChanged
		IF LEN( aRecordHeader ) > nLinesLeft
			EjectPage()

			IF ( aReportData[ RP_PLAIN ] )
				nLinesLeft := 1000
			ELSE
				ReportHeader()
			ENDIF

		ENDIF

		AEVAL( aRecordHeader, { | HeaderLine | ;
			PrintIt( SPACE( aReportData[ RP_LMARGIN ] ) + translate_charset(set("DBF_CHARSET"),host_charset(),HeaderLine ) ) } )

		aRecordHeader := {}

		EjectPage()

		IF ( aReportData[ RP_PLAIN ] )
			nLinesLeft := 1000

		ELSE
			ReportHeader()

		ENDIF

	ENDIF

   // Add to aRecordHeader in the event that the group has changed and
   // new group headers need to be generated

   // Cycle through the groups
   FOR nGroup := 1 TO LEN(aReportData[RP_GROUPS])
      // If the group has changed
      IF MakeAStr(EVAL(aReportData[RP_GROUPS,nGroup,RG_EXP]),;
            aReportData[RP_GROUPS,nGroup,RG_TYPE]) == aGroupTotals[nGroup]
      ELSE
         AADD( aRecordHeader, "" )   // The blank line

// page eject after group
#ifndef OLDCODE
         //  put CRFF after group
         IF nGroup == 1 .AND. !lFirstPass .AND. !lAnySubTotals
            IF lEjectGrp := aReportData[ RP_GROUPS, nGroup, RG_AEJECT ]
               nLinesLeft  := 0
            ENDIF
         ENDIF
#endif

         AADD( aRecordHeader, IF(nGroup==1,"** ","* ") +;
               aReportData[RP_GROUPS,nGroup,RG_HEADER] + " " +;
               MakeAStr(EVAL(aReportData[RP_GROUPS,nGroup,RG_EXP]), ;
               aReportData[RP_GROUPS,nGroup,RG_TYPE]) )
      ENDIF
   NEXT

#ifndef OLDCODE
   lFirstPass := .F.
#endif

   // Is there anything in the record header?
   IF LEN( aRecordHeader ) > 0
      // Determine if aRecordHeader will fit on the current page.  If not,
      // start a new header
      IF LEN( aRecordHeader ) > nLinesLeft
         EjectPage()
         IF aReportData[ RP_PLAIN ]
            nLinesLeft := 1000
         ELSE
            ReportHeader()
         ENDIF
      ENDIF

      // Send aRecordHeader to the output device, resetting nLinesLeft
      AEVAL( aRecordHeader, { | HeaderLine | ;
              PrintIt( SPACE(aReportData[RP_LMARGIN])+ translate_charset(set("DBF_CHARSET"),host_charset(),HeaderLine ) ) } )

      nLinesLeft -= LEN( aRecordHeader )

      // Make sure it didn't hit the bottom margin
      IF nLinesLeft == 0
         EjectPage()
         IF aReportData[ RP_PLAIN ]
            nLinesLeft := 1000
         ELSE
            ReportHeader()
         ENDIF
      ENDIF
   ENDIF

   // Add to the group totals
   FOR nCol := 1 TO LEN(aReportData[RP_COLUMNS])
      // If this column should be totaled, do it
      IF aReportData[RP_COLUMNS,nCol,RC_TOTAL]
         // Cycle through the groups
         FOR nGroup := 1 TO LEN( aReportTotals ) - 1
            aReportTotals[nGroup+1,nCol] += ;
               EVAL( aReportData[RP_COLUMNS,nCol,RC_EXP] )
         NEXT
      ENDIF
   NEXT

   // Reset the group expressions in aGroupTotals
   FOR nGroup := 1 TO LEN(aReportData[RP_GROUPS])
      aGroupTotals[nGroup] := MakeAStr(EVAL(aReportData[RP_GROUPS,nGroup,RG_EXP]),;
                                    aReportData[RP_GROUPS,nGroup,RG_TYPE])
   NEXT

   // Only run through the record detail if this is NOT a summary report
   IF !aReportData[ RP_SUMMARY ]
      // Determine the max number of lines needed by each expression
     nMaxLines := 1
      FOR nCol := 1 TO LEN(aReportData[RP_COLUMNS])

         IF aReportData[RP_COLUMNS,nCol,RC_TYPE] $ "M"
            nMaxLines := MAX(XMLCOUNT(EVAL(aReportData[RP_COLUMNS,nCol,RC_EXP]),;
                         aReportData[RP_COLUMNS,nCol,RC_WIDTH]), nMaxLines)
         ELSEIF aReportData[RP_COLUMNS,nCol,RC_TYPE] $ "C"
            nMaxLines := MAX( XMLCOUNT( STRTRAN( EVAL( aReportData[RP_COLUMNS,nCol,RC_EXP]),;
                         ";", CHR(13)+CHR(10)),;
                         aReportData[RP_COLUMNS,nCol,RC_WIDTH]), nMaxLines)
         ENDIF
      NEXT

      // Size aRecordToPrint to the maximum number of lines it will need, then
      // fill it with nulls
      ASIZE( aRecordToPrint, nMaxLines )
      AFILL( aRecordToPrint, "" )

      // Load the current record into aRecordToPrint
      FOR nCol := 1 TO LEN(aReportData[RP_COLUMNS])
         FOR nLine := 1 TO nMaxLines
            // Check to see if it's a memo or character
            IF aReportData[RP_COLUMNS,nCol,RC_TYPE] $ "CM"
               //  Load the current line of the current column into cLine
               //  with multi-lines per record ";"- method
               IF aReportData[RP_COLUMNS,nCol,RC_TYPE] $ "C"
                  cLine := XMEMOLINE( TRIM( STRTRAN( EVAL(aReportData[RP_COLUMNS,nCol,RC_EXP]),;
                             ";", CHR(13)+CHR(10)) ),;
                             aReportData[RP_COLUMNS,nCol,RC_WIDTH], nLine )
               ELSE
                  cLine := XMEMOLINE(TRIM(EVAL(aReportData[RP_COLUMNS,nCol,RC_EXP])),;
                             aReportData[RP_COLUMNS,nCol,RC_WIDTH], nLine )
							 ENDIF
               cLine := PADR( cLine, aReportData[RP_COLUMNS,nCol,RC_WIDTH] )
            ELSE
               IF nLine == 1
                  cLine := TRANSFORM(EVAL(aReportData[RP_COLUMNS,nCol,RC_EXP]),;
                           aReportData[RP_COLUMNS,nCol,RC_PICT])
                  cLine := PADR( cLine, aReportData[RP_COLUMNS,nCol,RC_WIDTH] )
               ELSE
                  cLine := SPACE( aReportData[RP_COLUMNS,nCol,RC_WIDTH])
               ENDIF
            ENDIF
            // Add it to the existing report line
            IF nCol > 1
               aRecordToPrint[ nLine ] += " "
            ENDIF
            aRecordToPrint[ nLine ] += cLine
         NEXT
      NEXT

      // Determine if aRecordToPrint will fit on the current page
      IF LEN( aRecordToPrint ) > nLinesLeft
         // The record will not fit on the current page - will it fit on
         // a full page?  If not, break it up and print it.
         IF LEN( aRecordToPrint ) > nMaxLinesAvail
            // This record is HUGE!  Break it up...
            nLine := 1
            DO WHILE nLine < LEN( aRecordToPrint )
               PrintIt( SPACE(aReportData[RP_LMARGIN]) + aRecordToPrint[nLine] )
               nLine++
               nLinesLeft--
               IF nLinesLeft == 0
                  EjectPage()
                  IF aReportData[ RP_PLAIN ]
                     nLinesLeft := 1000
                  ELSE
                     ReportHeader()
                  ENDIF
               ENDIF
            ENDDO
         ELSE
            EjectPage()
            IF aReportData[ RP_PLAIN ]
               nLinesLeft := 1000
            ELSE
               ReportHeader()
            ENDIF
            AEVAL( aRecordToPrint, ;
               { | RecordLine | ;
                 PrintIt( SPACE(aReportData[RP_LMARGIN])+ RecordLine ) ;
               } ;
            )
            nLinesLeft -= LEN( aRecordToPrint )
         ENDIF
      ELSE
         // Send aRecordToPrint to the output device, resetting nLinesLeft
         AEVAL( aRecordToPrint, ;
            { | RecordLine | ;
              PrintIt( SPACE(aReportData[RP_LMARGIN])+ RecordLine ) ;
            } ;
         )
         nLinesLeft -= LEN( aRecordToPrint )
      ENDIF

#ifdef OLDCODE
      // Make sure it didn't hit the bottom margin
      IF nLinesLeft == 0
         EjectPage()
         IF aReportData[ RP_PLAIN ]
            nLinesLeft := 1000
         ELSE
            ReportHeader()
         ENDIF
      ENDIF
#endif

      // Tack on the spacing for double/triple/etc.
      IF aReportData[ RP_SPACING ] > 1

/*  Double space problem in REPORT FORM at the bottom of the page  */
#ifdef OLDCODE
         IF nLinesLeft > aReportData[ RP_SPACING ] - 1
#else
         IF nLinesLeft >= aReportData[ RP_SPACING ] - 1
#endif

            FOR nLine := 2 TO aReportData[ RP_SPACING ]
               PrintIt()
               nLinesLeft--
            NEXT
         ENDIF
      ENDIF

   ENDIF    // Was this a summary report?

   RETURN
