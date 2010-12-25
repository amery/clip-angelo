STATIC FUNCTION Settle( GetList, nPos, lInit )

   LOCAL nExitState

   IF ( nPos == 0 )
      nExitState := GE_DOWN
   ELSEIF ( nPos > 0 .AND. lInit)
      nExitState := GE_NOEXIT
   ELSE
      nExitState := GetList[ nPos ]:exitState
   ENDIF

   IF ( nExitState == GE_ESCAPE .OR. nExitState == GE_WRITE )
      RETURN ( 0 )
   ENDIF

   IF !( nExitState == GE_WHEN )
      oStatus:lastpos := nPos
      oStatus:bumptop := .F.
      oStatus:bumpbot := .F.
   ELSE
      IF oStatus:lastexit != 0
	 nExitState := oStatus:lastexit
      ELSEIF oStatus:nextget < oStatus:lastpos
	 nExitState := GE_UP
      ELSE
	 nExitState := GE_DOWN
      ENDIF

   ENDIF

   DO CASE
   CASE( nExitState == GE_UP )
      nPos--
   CASE( nExitState == GE_DOWN )
      nPos++
   CASE( nExitState == GE_TOP )
      nPos       := 1
      oStatus:bumptop  := .T.
      nExitState := GE_DOWN
   CASE( nExitState == GE_BOTTOM )
      nPos       := LEN( GetList )
      oStatus:bumpbot  := .T.
      nExitState := GE_UP
   CASE( nExitState == GE_ENTER )
      nPos++
   CASE( nExitState == GE_SHORTCUT )
      RETURN ( oStatus:nextget )
   CASE( nExitState == GE_MOUSEHIT )
      RETURN ( oStatus:nextget )
   ENDCASE

   IF ( nPos == 0 )
      IF ( !( ReadExit() ) .AND. !oStatus:bumpbot )
	 oStatus:bumptop  := .T.
	 nPos       := oStatus:lastpos
	 nExitState := GE_DOWN
      ENDIF

   ELSEIF ( nPos == len( GetList ) + 1 )
      IF ( !( ReadExit() ) .AND. !( nExitState == GE_ENTER ) .AND. ;
	   !oStatus:bumptop )
	 oStatus:bumpbot  := .T.
	 nPos       := oStatus:lastpos
	 nExitState := GE_UP
      ELSE
	 nPos := 0
      ENDIF

   ENDIF

   oStatus:lastexit := nExitState
   IF !( nPos == 0 )
      GetList[ nPos ]:exitState := nExitState
   ENDIF
RETURN ( nPos )
