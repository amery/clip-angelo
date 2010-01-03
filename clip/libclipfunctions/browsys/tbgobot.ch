/*****************************************************************************
*  tbGoBot( <aTBinfo> ) -> aTBinfo
*  go to the bottom of a browse.
*
*****************************************************************************/
FUNCTION tbGoBot( aTBinfo )
   LOCAL bBot, bSeek, bFilter, cAlias
      bBot   := aTBinfo[ TBR_BBLOCK ]
      IF ( bBot == NIL )
        bSeek   := aTBinfo[ TBR_SEEKBLOCK ]
        IF (bSeek != NIL )
          Eval( bSeek, aTBinfo )
          cAlias  := aTBinfo[ TBR_REFERENCE ]
          bFilter   := aTBinfo[ TBR_FILTERBLOCK ]
			    IF ( ! ( cAlias )->( eof() ) )
		        WHILE ( .T. )
   		        ( cAlias ) -> ( dbSkip( 1 ) )
			        IF ( ( ( cAlias )->( eof() ) ) .OR. ;
                      ( ! Eval( bFilter, aTBinfo ) ) )
      			        ( cAlias )->( dbSkip( -1 ) )
                    EXIT
			        ENDIF
		        ENDDO
          ENDIF
        ELSE
          ( aTBinfo[ TBR_REFERENCE ] )->( dbGoBottom() )
        ENDIF
      ELSE
         Eval( bBot, aTBinfo )
      ENDIF

   RETURN ( aTBinfo )
