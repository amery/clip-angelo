/*****************************************************************************
*  tbGoTop( <aTBinfo> ) -> aTBinfo
*  go to the top of a browse.
*
*****************************************************************************/
FUNCTION tbGoTop( aTBinfo )
   LOCAL bSeek
      bSeek   := aTBinfo[ TBR_SEEKBLOCK ]
      IF ( bSeek == NIL )
         ( aTBinfo[ TBR_REFERENCE ] )->( dbGoTop() )
      ELSE
         Eval( bSeek, aTBinfo )
      ENDIF

   RETURN ( aTBinfo )
