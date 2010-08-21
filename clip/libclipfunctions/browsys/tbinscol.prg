/*****************************************************************************
*  tbInsCol( <aTBinfo>, <aColInfo>, <nPosition> ) -> aTBinfo
*  insert a column into a browse.
*
*****************************************************************************/
FUNCTION tbInsCol( aTBinfo, aColInfo, nPosition )
   RETURN ( MakeColumn( aTBinfo, aColInfo, nPosition ) )
