/*****************************************************************************
*  tbDelCol( <aTBinfo>, <nPosition> ) -> aTBinfo
*  delete a column from a browse.
*
*****************************************************************************/
FUNCTION tbDelCol( aTBinfo, nPosition )
   local aColInfo

      IF ( nPosition <= aTBinfo[ TBR_BROWSE ]:ColCount )
         aTBinfo[ TBR_BROWSE ]:DelColumn( nPosition )
         aColInfo := aTBInfo[ TBR_COLINFO ]
         aDel( aColInfo, nPosition )
         aSize( aColInfo, Len( aColInfo ) - 1 )
      ENDIF

   RETURN ( aTBinfo )
