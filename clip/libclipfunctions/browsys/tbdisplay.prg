/*****************************************************************************
*  tbDisplay( <aTBinfo>, <lMode> ) -> aTBinfo
*  display a browse window.
*
*****************************************************************************/
FUNCTION tbDisplay( aTBinfo, lMode )


      IF ( ! ValType( lMode ) == "L" )
         lMode := .F.
      ENDIF

      IF ( lMode )
         aTBinfo[ TBR_BROWSE ]:Invalidate()

      ELSE
         aTBinfo[ TBR_BROWSE ]:RefreshAll()

      ENDIF

      aTBinfo[ TBR_BROWSE ]:ForceStable()

   RETURN ( aTBInfo )

