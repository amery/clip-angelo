/*****************************************************************************
*  tbMouse ( <oBrowse>, <nRow>, <nColumn> ) -> oBrowse
*  perform mouse related activities on a tBrowse object.
*
*****************************************************************************/
FUNCTION tbMouse( oBrowse, nRow, nCol )
   LOCAL nCount

      IF ( oBrowse:HitTest( nRow, nCol ) == HTCELL )

         nCount := oBrowse:mRowPos - oBrowse:RowPos
         WHILE ( nCount < 0 )
            nCount ++
            oBrowse:Up()
         ENDDO

         WHILE ( nCount > 0 )
            nCount --
            oBrowse:Down()
         ENDDO

         nCount := oBrowse:mColPos - oBrowse:ColPos
         WHILE ( nCount < 0 )
            nCount ++
            oBrowse:Left()
         ENDDO

         WHILE ( nCount > 0 )
            nCount --
            oBrowse:Right()
         ENDDO

         RETURN ( TBR_CONTINUE )
      ENDIF

   RETURN ( TBR_EXCEPTION )
