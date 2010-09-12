/*****************************************************************************
*  tbAddCol( <aTBinfo>, <aColInfo> ) -> aTBinfo
*  add a column to a browse.
*
*****************************************************************************/
FUNCTION tbAddCol( aTBinfo, aColInfo )
   RETURN ( MakeColumn( aTBinfo, aColInfo, NIL ) )
