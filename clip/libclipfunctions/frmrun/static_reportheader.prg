/***
*
*  ReportHeader()
*
*/
STATIC PROCEDURE ReportHeader
   LOCAL nLinesInHeader := 0
   LOCAL aPageHeader    := {}
   LOCAL nHeadingLength := aReportData[RP_WIDTH] - aReportData[RP_LMARGIN] - 30
   LOCAL nCol, nLine, nMaxColLength, nGroup, cHeader
   LOCAL nHeadLine            // lines in a single heading
   LOCAL nRPageSize           // width of report after subtracting right margin
   LOCAL aTempPgHeader        // temporary page header array
   LOCAL nHeadSize


   nRPageSize := aReportData[RP_WIDTH] - aReportData[RP_RMARGIN]

   //  Header width should be less then 255 characters.
   nHeadSize := MIN (nRPageSize, 254)

   // Create the header and drop it into aPageHeader

   // Start with the heading
   IF !aReportData[ RP_PLAIN ]           // If not a plain paper report, build
      IF aReportData[RP_HEADING] == ""   // the heading
         AADD( aPageHeader, NationMsg(_RF_PAGENO) + STR(nPageNumber,6) )

      ELSE
         aTempPgHeader := ParseHeader( aReportData[ RP_HEADING ], ;
            Occurs( ";", aReportData[ RP_HEADING ] ) + 1 )

         FOR nLine := 1 TO LEN( aTempPgHeader )
            // determine number of lines in header given current report dimensions
            nLinesInHeader := MAX( XMLCOUNT( LTRIM( aTempPgHeader[ nLine ] ), ;
               nHeadingLength ), 1 )

            // extract lines and add to array
            FOR nHeadLine := 1 TO nLinesInHeader

               AADD( aPageHeader, SPACE( 15 ) + ;
                  PADC( TRIM( XMEMOLINE( LTRIM( aTempPgHeader[ nLine ] ),;
                  nHeadingLength, nHeadLine ) ), nHeadingLength ) )

            NEXT nHeadLine

         NEXT nLine
         aPageHeader[ 1 ] := STUFF( aPageHeader[ 1 ], 1, 14, ;
                                    NationMsg(_RF_PAGENO) + STR(nPageNumber,6) )

      ENDIF
      AADD( aPageHeader, DTOC(DATE()) )

   ENDIF

   // Tack on the actual header from the FRM
   FOR nLine := 1 TO LEN( aReportData[RP_HEADER] )
      // determine number of lines in header given current report dimensions

      nLinesInHeader := MAX( XMLCOUNT( LTRIM( aReportData[RP_HEADER, ;
         nLine ] ), nHeadSize ), 1 )

      // extract lines and add to array
      FOR nHeadLine := 1 TO nLinesInHeader

         cHeader := TRIM( XMEMOLINE( LTRIM( aReportData[ RP_HEADER, nLine ] ),;
            nHeadSize, nHeadLine) )

         AADD( aPageHeader, SPACE( ( nRPageSize - aReportData[ RP_LMARGIN ] - ;
            LEN( cHeader ) ) / 2 ) + cHeader )
      NEXT nHeadLine

   NEXT nLine

#ifdef S87_COMPAT
   // S87 compat.
   AADD( aPageHeader, "" )
#endif

   // Now add the column headings
   nLinesInHeader := LEN( aPageHeader )

   // Determine the longest column header
   nMaxColLength := 0
   FOR nCol := 1 TO LEN( aReportData[ RP_COLUMNS ] )
       nMaxColLength := MAX( LEN(aReportData[RP_COLUMNS,nCol,RC_HEADER]), ;
                             nMaxColLength )
   NEXT
   FOR nCol := 1 TO LEN( aReportData[ RP_COLUMNS ] )
      ASIZE( aReportData[RP_COLUMNS,nCol,RC_HEADER], nMaxColLength )
   NEXT

   FOR nLine := 1 TO nMaxColLength
      AADD( aPageHeader, "" )
   NEXT

   FOR nCol := 1 TO LEN(aReportData[RP_COLUMNS])    // Cycle through the columns
      FOR nLine := 1 TO nMaxColLength
         IF nCol > 1
            aPageHeader[ nLinesInHeader + nLine ] += " "
         ENDIF
         IF aReportData[RP_COLUMNS,nCol,RC_HEADER,nLine] == NIL
            aPageHeader[ nLinesInHeader + nLine ] += ;
                           SPACE( aReportData[RP_COLUMNS,nCol,RC_WIDTH] )
         ELSE
            IF aReportData[RP_COLUMNS,nCol,RC_TYPE] == "N"
               aPageHeader[ nLinesInHeader + nLine ] += ;
                           PADL(aReportData[RP_COLUMNS,nCol,RC_HEADER,nLine],;
                           aReportData[RP_COLUMNS,nCol,RC_WIDTH])
            ELSE
               aPageHeader[ nLinesInHeader + nLine ] += ;
                           PADR(aReportData[RP_COLUMNS,nCol,RC_HEADER,nLine],;
                           aReportData[RP_COLUMNS,nCol,RC_WIDTH])
            ENDIF
         ENDIF
      NEXT
   NEXT

   // Insert the two blank lines between the heading and the actual data
   AADD( aPageHeader, "" )
   AADD( aPageHeader, "" )
   AEVAL( aPageHeader, { | HeaderLine | ;
         PrintIt( SPACE(aReportData[RP_LMARGIN])+ translate_charset(set("DBF_CHARSET"),host_charset(),HeaderLine )) } )

   // Set the page number and number of available lines
   nPageNumber++

	// adjust the line count to account for Summer '87 behavior
   nLinesLeft := aReportData[RP_LINES] - LEN( aPageHeader )
   nMaxLinesAvail := aReportData[RP_LINES] - LEN( aPageHeader )

   RETURN
