/*********************************************************************
*  tbCreate( <uOwner>, <nTop>, <nLeft>, <nBottom>, <nRight>
*              [, <cColorInfo>] ) -> aTBinfo
*  create and initialize a browse window.
*
*********************************************************************
*/
FUNCTION tbCreate( uOwner, nTop, nLeft, nBottom, nRight, cColorInfo, cBorder, lAppend, lUpdate )
   LOCAL oBrowse, aTBinfo

      oBrowse := tBrowseNew( nTop, nLeft, nBottom, nRight )
      oBrowse:AutoLite      := .F.
      oBrowse:HeadSep       := "�"
      oBrowse:ColSep        := "�"
      oBrowse:SkipBlock     := { | x | tbSkip( aTBInfo, x ) }

      IF ( ValType( uOwner ) == "A" )
         oBrowse:GoTopBlock    := { || aTBinfo[ TBR_POSITION ] := 1 }
         oBrowse:GoBottomBlock := { || aTBinfo[ TBR_POSITION ] := Len( aTBinfo[ TBR_REFERENCE ] ) }

      ELSE
         oBrowse:GoTopBlock    := { || tbGoTop( aTBInfo ) }
         oBrowse:GoBottomBlock := { || tbGoBot( aTBInfo ) }

      ENDIF

      IF ( ValType( cColorInfo ) == "C" )
         oBrowse:ColorSpec := cColorInfo
      ENDIF

      IF ( ValType( cBorder ) == "C" )
         oBrowse:Border := cBorder
      ENDIF

      IF ( ValType( lAppend ) == "L" )
         oBrowse:SetStyle( TBR_APPEND, lAppend )

      ELSE
         lAppend := .F.

      ENDIF

      IF ( ValType( lUpdate ) == "L" )
         oBrowse:SetStyle( TBR_MODIFY, lUpdate )

      ELSE
         lUpdate := .F.

      ENDIF

      IF ( lUpdate )
         oBrowse:SetKey( 0, { | oB, nK | tbEditCell( oB, nK ) } )
      ENDIF

      aTBinfo := ARRAY( TBR_ELEMENTS )

      aTBinfo[ TBR_BROWSE      ] := oBrowse
      aTBinfo[ TBR_SAVESCREEN  ] := ""
      aTBinfo[ TBR_REFERENCE   ] := uOwner
      aTBinfo[ TBR_COLINFO     ] := {}
      aTBinfo[ TBR_FILTERBLOCK ] := { || .T. }

      IF ( ( SET( _SET_VIDEOMODE ) == LLG_VIDEO_TXT ) .OR.;
           ( SET( _SET_VIDEOMODE ) == NIL ) )

         aTBinfo[ TBR_SAVESCREEN ] := CHR( oBrowse:nTop    )     + ;
                                      CHR( oBrowse:nLeft   )     + ;
                                      CHR( oBrowse:nBottom )     + ;
                                      CHR( oBrowse:nRight  )     + ;
                                      SAVESCREEN( oBrowse:nTop   , ;
                                                  oBrowse:nLeft  , ;
                                                  oBrowse:nBottom, ;
                                                  oBrowse:nRight )
      ELSE     // Graphic mode

         aTBinfo[ TBR_SAVESCREEN ] := SaveScreen( oBrowse:nTop,    ;
                                                  oBrowse:nLeft,   ;
                                                  oBrowse:nBottom, ;
                                                  oBrowse:nRight )
         AADD( aTBinfo[ TBR_SAVESCREEN ], oBrowse:nTop )
         AADD( aTBinfo[ TBR_SAVESCREEN ], oBrowse:nLeft )
         AADD( aTBinfo[ TBR_SAVESCREEN ], oBrowse:nBottom )
         AADD( aTBinfo[ TBR_SAVESCREEN ], oBrowse:nRight )

      ENDIF

  RETURN ( aTBinfo )
