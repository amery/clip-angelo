/*****************************************************************************
*  tbSblock( <aTBinfo>, <bBlock> ) -> bBlock
*  get/set the seek block.
*
*****************************************************************************/
FUNCTION tbSblock( aTBinfo, bBlock )
   LOCAL bReturn

      bReturn := aTBinfo[ TBR_SEEKBLOCK ]

      IF ( ValType( bBlock ) == "B" )
         aTBinfo[ TBR_SEEKBLOCK ] := bBlock
      ENDIF

   RETURN ( bReturn )
