FUNCTION Accelerator( GetList, nKey, oMsg )
   LOCAL nGet, oGet, nHotPos, cKey, cCaption, nStart, nEnd
   LOCAL nIteration, lGUI

      IF ( ( nKey >= K_ALT_Q ) .AND. ( nKey <= K_ALT_P ) )
	 cKey := SUBSTR( "qwertyuiop", nKey - K_ALT_Q + 1, 1 )

      ELSEIF ( ( nKey >= K_ALT_A ) .AND. ( nKey <= K_ALT_L ) )
	 cKey := SUBSTR( "asdfghjkl", nKey - K_ALT_A + 1, 1 )

      ELSEIF ( ( nKey >= K_ALT_Z ) .AND. ( nKey <= K_ALT_M ) )
	 cKey := SUBSTR( "zxcvbnm", nKey - K_ALT_Z + 1, 1 )

      ELSEIF ( ( nKey >= K_ALT_1 ) .AND. ( nKey <= K_ALT_0 ) )
	 cKey := SUBSTR( "1234567890", nKey - K_ALT_1 + 1, 1 )

      ELSE
	 RETURN ( 0 )

      ENDIF

      nStart := oStatus:pos + 1
      nEnd   := LEN( GetList )

      FOR nIteration := 1 TO 2
	 FOR nGet := nStart TO nEnd
	    oGet  := GetList[ nGet ]

	    IF ( VALTYPE( oGet:Control ) == "O" .AND. ;
			  oGet:Control:ClassName() != "TBROWSE" )
	       cCaption := oGet:Control:Caption
	    ELSEIF "CAPTION" $ oGet
	       cCaption := oGet:Caption
	    else
		cCaption := ""
	    ENDIF

	    IF ( ( nHotPos := AT( "&", cCaption ) ) == 0 )
	    ELSEIF ( nHotPos == LEN( cCaption ) )
	    ELSEIF ( LOWER( SUBSTR( cCaption, nHotPos + 1, 1 ) ) == cKey )

		lGUI := ( VALTYPE( Getlist[ oStatus:pos ]:Control ) == "O" )
		IF lGUI .AND. !( GUIPostValidate( GetList[ oStatus:pos ], ;
		    GetList[ oStatus:pos ]:Control, oMsg  ) )
		    RETURN 0
		ELSEIF !lGUI .AND. !( GetPostValidate( GetList[ oStatus:pos ], ;
		       oMsg ) )
		       RETURN 0
		ENDIF

		lGUI := ( VALTYPE( oGet:Control ) == "O" )
		IF lGUI .AND. !( GUIPreValidate( oGet, oGet:Control, oMsg  ) )
		    RETURN nGet
		ELSEIF !lGUI .AND. !( GetPreValidate( oGet, oMsg ) )
		    RETURN nGet
		ENDIF

		RETURN ( nGet )
	    ENDIF
	 NEXT

	 nStart := 1
	 nEnd   := oStatus:pos - 1

      NEXT
RETURN ( 0 )
