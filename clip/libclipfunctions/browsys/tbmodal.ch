/*****************************************************************************
*  tbModal( <oBrowse> ) -> nKey
*  give input focus to a browse.
*
*****************************************************************************/
FUNCTION tbModal( oBrowse )
   LOCAL nKey, nSaveCursor, lAutoLite

      nSaveCursor      := SetCursor( SC_NONE )
      lAutoLite        := oBrowse:AutoLite
      oBrowse:AutoLite := .T.

      WHILE ( .T. )
         WHILE ( ( ( nKey := Inkey() ) == 0 ) .AND. ( ! oBrowse:Stabilize() ) )
         ENDDO

         IF ( nKey == 0 )
            /* nKey is 0, so the browse must be stable. */
            IF ( ( oBrowse:hitBottom ) .AND. ;
                 ( oBrowse:SetStyle( TBR_APPEND ) ) .AND. ;
                 ( ! oBrowse:SetStyle( TBR_APPENDING ) ) )
               oBrowse:SetStyle( TBR_APPENDING, .T. )
               nKey := K_DOWN

            ELSE
               nKey := Inkey( 0 )

            ENDIF
         ENDIF

         IF ( ( nKey == K_CTRL_PGDN ) .OR. ( nKey == K_CTRL_PGUP ) )
            oBrowse:SetStyle( TBR_APPENDING, .F. )

         ELSEIF ( ( nKey == K_UP ) .OR. ( nKey == K_PGUP ) )
            IF ( oBrowse:SetStyle( TBR_APPENDING ) )
               oBrowse:SetStyle( TBR_APPENDING, .F. )
               oBrowse:refreshAll()
            ENDIF

         ELSEIF ( ( nKey == K_LBUTTONDOWN ) .AND. ;
                  ( oBrowse:mRowPos != oBrowse:RowPos ) .AND. ;
                  ( oBrowse:HitTest( mRow(), mCol() ) == HTCELL ) )
            IF ( oBrowse:SetStyle( TBR_APPENDING ) )
               oBrowse:SetStyle( TBR_APPENDING, .F. )
               oBrowse:refreshAll()
            ENDIF
         ENDIF

         IF ( oBrowse:ApplyKey( nKey ) == TBR_EXIT )
            EXIT
         ENDIF
      ENDDO

      oBrowse:AutoLite := lAutoLite
      oBrowse:deHilite()

      SetCursor( nSaveCursor )

   RETURN ( nKey )
