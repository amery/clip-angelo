// Demonstration of array dimensioning under Windows            �
/*
File Name  : ATEST.PRG

Description: Demonstration of array dimensioning under Windows

*/

#INCLUDE <ci_FiveWin.ch>

Static oWnd
//-------------------------------------------------------------//

FUNCTION Main()
   DEFINE WINDOW oWnd                                          ;
      TITLE "Massive Dialog Box use test";
      COLOR CLR_YELLOW

   if GetFwDriver()=="GTK"
   @ 2,  2 BUTTON "&Start"                                     ;
      SIZE 80, 20                                              ;
      ACTION ( MsgMeter( { | oMeter, oText, oDlg, lEnd, oBtn |       ;
      ArrayTest( oMeter, oText, @lEnd ) }                     , ;
      "Element n� xxxx"                                       , ;
      "Adding 2000 elements to a Clipper array..." ))
   else
   @ 2,  2 BUTTON "&Start"                                     ;
      SIZE 80, 20                                              ;
      ACTION MsgAbout()
   endif

   ACTIVATE WINDOW oWnd VALID MsgYesNo("Uveren??")

RETURN( NIL )

//-------------------------------------------------------------//

FUNCTION ArrayTest( oMeter, oText, lEnd )

   LOCAL aTest := {}
   LOCAL n     := 1
   LOCAL nOld  := 1

   oMeter:SetTotal(2000)
   oText:SetText( OemToAnsi( "Element n�: " ) + str( n, 4 ) )

   WHILE n <= oMeter:nTotal .and. ! lEnd
      aadd( aTest, "" )

      oMeter:Set( n++ )

      IF n - nOld == 10
	 oText:SetText( OemToAnsi( "Element n�: " ) +           ;
	    str( n - 1, 4 ) )
	 nOld := n
      ENDIF


      /* Let Windows process pending messages */
      SysRefresh()

   ENDDO

   IF lEnd
      MsgAlert( "Sample cancelled" )
   ELSE
      MsgInfo( len( aTest ), "Array Length" )
   ENDIF

RETURN( NIL )
//-------------------------------------------------------------//
