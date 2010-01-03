/*****************************************************************************
*  FieldNPut( <cAlias>, <cField>, <uData> ) -> lSuccess
*  create a network aware fieldput() function.
*
*****************************************************************************/
FUNCTION FieldNPut( cAlias, cField, uData )
   LOCAL nField, nSeconds

      IF ( Select( cAlias ) == 0 )
         RETURN ( .F. )

      ELSEIF ( ( cAlias )->( ( nField := FieldPos( cField ) ) ) == 0 )
         RETURN ( .F. )

      ENDIF

      nSeconds := Seconds()

      WHILE ( ! ( cAlias )->( dbRlock() ) )
         IF ( Seconds() < nSeconds )
         ELSEIF ( Alert( "ERROR: Unable to Save Information", ;
                         { "Retry", "Cancel" } ) != 1 )
            RETURN ( .F. )

         ELSE
            nSeconds := Seconds() + 20

         ENDIF

         Inkey( .2 )
      ENDDO

      ( cAlias )->( FieldPut( nField, uData ) )
      ( cAlias )->( dbrUnlock( ( cAlias )->( Recno() ) ) )


   RETURN ( .T. )
