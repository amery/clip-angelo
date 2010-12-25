FUNCTION ReadModal( _GetList, nPos, oMenu, nMsgRow, nMsgLeft, ;
					  nMsgRight, cMsgColor )
   LOCAL oGet, aMode, oMsg
   LOCAL aSavGetSysVars
   LOCAL cMsg := NIL
   LOCAL nSaveCursor

   if oStatus==NIL
      __getSysInit()
   endif
   //private getlist := {}

   IF ( VALTYPE( oStatus:format ) == "B" )
      EVAL( oStatus:format )
   ENDIF

   IF ( EMPTY( _GetList ) )
      SETPOS( MAXROW() - 1, 0 )
      RETURN (.F.)
   ENDIF

   nSaveCursor := SETCURSOR( SC_NONE )
   aSavGetSysVars := ClearGetSysVars()
   oStatus:svccursor := SETCURSOR( SC_NONE )

   oStatus:procname := PROCNAME( 1 )
   oStatus:procline := PROCLINE( 1 )

   oMsg := map()
   oMsg:flag := NIL
   oMsg:row  := nMsgRow
   oMsg:left := nMsgLeft
   oMsg:right:= nMsgRight
   oMsg:color:= cMsgColor
   oMsg:back1:= NIL
   oMsg:back2:= NIL
   oMsg:fore := NIL
   oMsg:fontcol:= NIL
   oMsg:fontrow:= NIL

   IF ( VALTYPE( nPos ) == "N" )
      oStatus:pos := Settle( _GetList, nPos, .T. )
   ELSE
      oStatus:pos := Settle( _GetList, 0, .T. )
   ENDIF

   IF ( oMsg:flag :=  ( VALTYPE( oMsg:row   ) + ;
			      VALTYPE( oMsg:left ) + ;
			      VALTYPE( oMsg:right ) == "NNN" ) )

      IF !( VALTYPE( oMsg:color ) == "C" )
	 oMsg:color := GetClrPair( SetColor(), 1 )
      ENDIF

      @ oMsg:row, oMsg:left CLEAR TO ;
	oMsg:row, oMsg:right

      oStatus:scrvmsg := SaveScreen( oMsg:row, ;
	   oMsg:left, oMsg:row, oMsg:right )

   ENDIF

   oStatus:nextget := 0
   oStatus:hitcode := 0
   oStatus:menuid  := 0

   WHILE !( oStatus:pos == 0 )

      oGet := _GetList[ oStatus:pos ]
      aGetList := _GetList
	 PostActiveGet( oGet )

      IF ( VALTYPE( oGet:reader ) == "B" )
	 EVAL( oGet:reader, oGet, _GetList, oMenu, oMsg )
      ELSE
	 GetReader( oGet, _GetList, oMenu, oMsg ) // Use standard reader
      ENDIF
      oStatus:pos := Settle( _GetList, oStatus:pos, .F. )

   ENDDO

   RestoreGetSysVars( aSavGetSysVars )
   SETPOS( MAXROW() - 1, 0 )
   SETCURSOR( oStatus:svccursor )
RETURN ( oStatus:updated )
