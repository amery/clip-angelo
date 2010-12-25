/*****************************************************************************
*  tbBblock( <aTBinfo>, <bBlock> ) -> bBlock
*  get/set the gobottom block.
*
*****************************************************************************/
FUNCTION tbBblock( aTBinfo, bBlock )
   LOCAL nPosition, bReturn

      bReturn := aTBinfo[ TBR_BBLOCK ]

      IF ( ValType( bBlock ) == "B" )
         aTBinfo[ TBR_BBLOCK ] := bBlock
      ENDIF

   RETURN ( bReturn )
