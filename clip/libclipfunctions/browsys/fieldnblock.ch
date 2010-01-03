/*****************************************************************************
*  FieldNBlock( <cAlias>, <cField> ) -> bGetSet
*  create a network aware get/set block.
*
*****************************************************************************/
FUNCTION FieldNBlock( cAlias, cField )
   LOCAL nField

      IF ( Select( cAlias ) == 0 )
         RETURN ( NIL )

      ELSEIF ( ( cAlias )->( ( nField := FieldPos( cField ) ) ) == 0 )
         RETURN ( NIL )

      ENDIF

   RETURN ( { | x | IIF( x == NIL, ( cAlias )->( FieldGet( nField ) ), ;
                                   FieldNPut( cAlias, cField, x ) ) } )

