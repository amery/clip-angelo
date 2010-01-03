/*****************************************************************************
*   InsToggle()
*
*   Toggle the global insert mode and the cursor shape.
*
*****************************************************************************/
STATIC FUNCTIOn InsToggle()

      IF ( ReadInsert() )
         ReadInsert( .F. )
         SetCursor( SC_NORMAL )

      ELSE
         ReadInsert( .T. )
         SetCursor( SC_INSERT )

      ENDIF

   RETURN ( NIL )
