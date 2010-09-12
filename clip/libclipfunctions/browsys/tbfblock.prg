/*****************************************************************************
*  tbFblock( <aTBinfo>, <bBlock> ) -> bBlock
*  get/set the filter block.
*
*****************************************************************************/
FUNCTION tbFblock( aTBinfo, bBlock )
   LOCAL bReturn

      bReturn := aTBinfo[ TBR_FILTERBLOCK ]

      IF ( ValType( bBlock ) == "B" )
         aTBinfo[ TBR_FILTERBLOCK ] := bBlock
      ENDIF

   RETURN ( bReturn )
