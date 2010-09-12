/*****************************************************************************
*  tbSkip( <aTBinfo>, <nRows> ) -> nRowsMoved
*  skip one or more rows in a browse.
*
*****************************************************************************/
FUNCTION tbSkip( aTBinfo, nAmount )
   LOCAL nPos, nLength, nReturn, nDirection, cAlias, bFilter, lIsAppend

      bFilter   := aTBinfo[ TBR_FILTERBLOCK ]
      lIsAppend := aTBinfo[ TBR_BROWSE ]:SetStyle( TBR_APPENDING )

      IF ( ValType( aTBinfo[ TBR_REFERENCE ] ) == "A" )
         nPos := aTBInfo[ TBR_POSITION ] + nAmount

         IF ( nPos < 1 )
            nAmount += ( ABS( nPos ) + 1 )
            nPos := 1

         ELSEIF ( nPos > ( nLength := LEN( ( aTBInfo[ TBR_REFERENCE ] ) ) ) )
            nAmount -= ( nLength - nPos )
            nPos := nLength

         ENDIF

         aTBInfo[ TBR_POSITION ] := nPos

      ELSE
	      nReturn := 0
         cAlias  := aTBinfo[ TBR_REFERENCE ]

	      IF ( LastRec() != 0 )
		      IF ( nAmount == 0 )
		         ( cAlias )->( dbSkip( 0 ) )

		      ELSEIF ( ( nAmount > 0 ) .AND. ( ! ( cAlias )->( Eof() ) ) )
		         WHILE ( nReturn < nAmount )
   		         ( cAlias )->( dbSkip( 1 ) )

			         IF ( ( ( cAlias )->( eof() ) ) .OR. ;
                       ( ! Eval( bFilter, aTBinfo ) ) )

                     IF ( lIsAppend )
			                  IF ( ! ( cAlias )->( eof() ) )
         			            ( cAlias )->( dbGoBottom() )
         			            ( cAlias )->( dbSkip( 1 ) )
                        ENDIF
                        nReturn ++

                     ELSE
      			         ( cAlias )->( dbSkip( -1 ) )

                     ENDIF

				         EXIT
			         ENDIF

			         nReturn ++
		         ENDDO

		      ELSEIF ( nAmount < 0 )
               IF ( ( cAlias )->( Eof() ) )
                  Eval( aTBinfo[ TBR_BROWSE ]:GoBottomBlock )
                  nReturn --
               ENDIF
   		      WHILE ( nReturn > nAmount )
      		      ( cAlias )->( dbSkip( -1 ) )
		   	      IF ( ( cAlias )->( bof() ) )
				         EXIT

		   	      ELSEIF ( ! Eval( bFilter, aTBinfo ) )
         			   ( cAlias )->( dbSkip( 1 ) )
				         EXIT

			         ENDIF

			         nReturn --
   		      ENDDO
		      ENDIF
	      ENDIF

         aTBInfo[ TBR_POSITION ] := ( cAlias )->( RECNO() )
      ENDIF

   RETURN ( nReturn )
