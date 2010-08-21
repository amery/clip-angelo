/*************/
FUNCTION HitTest( GetList, MouseRow, MouseCol, oMsg )
   LOCAL nCount, nTotal, lGUI

   oStatus:nextget := 0
   nTotal  := LEN( GetList )

   FOR nCount := 1 TO nTotal
      IF ( ( oStatus:hitcode := ;
	 GetList[ nCount ]:HitTest( MouseRow, MouseCol ) ) != HTNOWHERE )
	 oStatus:nextget := nCount
	 EXIT
      ENDIF
   NEXT

   IF !( oStatus:nextget == 0 )
      lGUI := ( VALTYPE( Getlist[ oStatus:pos ]:Control ) == "O" )
      IF lGUI .AND. !( GUIPostValidate( GetList[ oStatus:pos ], ;
		GetList[ oStatus:pos ]:Control, oMsg  ) )
	 oStatus:nextget := 0
	 RETURN 0
      ELSEIF !lGUI .AND. !( GetPostValidate( GetList[ oStatus:pos ], ;
	 oMsg ) )
	 oStatus:nextget := 0
	 RETURN 0
      ENDIF

      lGUI := ( VALTYPE( Getlist[ oStatus:nextget ]:Control ) == "O" )
      IF lGUI .AND. !( GUIPreValidate( GetList[ oStatus:nextget ], ;
		GetList[ oStatus:nextget ]:Control, oMsg  ) )
	 oStatus:nextget := 0
	 RETURN oStatus:nextget
      ELSEIF !lGUI .AND. !( GetPreValidate( GetList[ oStatus:nextget ], ;
	 oMsg ) )
	 oStatus:nextget := 0
	 RETURN oStatus:nextget
      ENDIF
      RETURN oStatus:nextget
   ENDIF

RETURN 0
