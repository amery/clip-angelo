/*****************************************************************************
*  tbEditCell( <oBrowse>, <nKey> ) -> uNewValue
*  modify the current cell.
*
*****************************************************************************/
FUNCTION tbEditCell( oBrowse, nKey )
   LOCAL bSaveInsert, GetList, lAppend, nLength, nSaveCursor, oColumn, ;
         uNewKey, uSavKey
      IF ( ! oBrowse:SetStyle( TBR_MODIFY ) )
         RETURN ( TBR_EXCEPTION )
      ENDIF

      oColumn  := oBrowse:GetColumn( oBrowse:ColPos )

      IF ( ! oColumn:SetStyle( TBC_READWRITE ) )
         RETURN ( TBR_EXCEPTION )
      ENDIF

      IF ( nKey == K_ENTER )
      ELSEIF ( nKey < 32 )
         RETURN ( TBR_EXCEPTION )

      ELSEIF ( nKey > 255 )
         RETURN ( TBR_EXCEPTION )

      ENDIF

      IF ( Eof() )
         lAppend := .T.

      ELSE
         lAppend := oBrowse:SetStyle( TBR_APPENDING )

      ENDIF

      oBrowse:ForceStable()

      IF ( lAppend )
         dbAppend()
      ENDIF

      uSavKey := iif( Empty( IndexKey() ), NIL, &( IndexKey() ) )
      nLength := oBrowse:colWidth( oBrowse:colPos )

      KeyBoard( Chr( nKey ) )

      /* Create a corresponding GET. */
      GetList := { getnew( row(), col(),        ;
                           oColumn:block,       ;
                           oColumn:heading,     ;
                           oColumn:picture,     ;
                           oBrowse:colorSpec ) }

      GetList[ 1 ]:PreBlock  := oColumn:PreBlock
      GetList[ 1 ]:PostBlock := oColumn:PostBlock

      bSaveInsert  := setkey( K_INS, { || InsToggle() } )
      nSaveCursor := SetCursor( iif( ReadInsert(), SC_INSERT, SC_NORMAL ) )

      ReadModal( GetList )

      setcursor( nSaveCursor )
      setkey( K_INS, bSaveInsert )

      oBrowse:SetStyle( TBR_APPENDING, .F. )

      // Get the record's key value (or NIL) after the GET
      uNewKey := if( empty( indexkey() ), NIL, &( indexkey() ) )

      oBrowse:Invalidate()
      oBrowse:RefreshAll():ForceStable()

      IF ( ! ( uNewKey == uSavKey ) .or. ( lAppend .and. uNewKey != NIL ) )
         /* perform a complete refresh. */
         oBrowse:refreshAll():forceStable()

      ENDIF

      /* Check the exit key from the read. */
      nKey := LastKey()
      IF ( ( nKey == K_UP   ) .OR. ;
           ( nKey == K_DOWN ) .OR. ;
           ( nKey == K_PGUP ) .OR. ;
           ( nKey == K_PGDN )      )

         /* Yea, yea. What's a guy to do? */
         keyboard( chr( nKey ) )
      ENDIF

   RETURN ( TBR_CONTINUE )
