// This sample shows how to easily implement 'tool-tips'
// in FiveWin!
/*
File Name  : TOOLTIP.PRG

Description: This sample shows how to easily implement
	     'tool-tips' in FiveWin!

*/

#INCLUDE "ci_FiveWin.ch"

STATIC oWnd

//-------------------------------------------------------------//

FUNCTION Main()
   LOCAL oBar, oBtn

   DEFINE WINDOW oWnd                                           ;
      TITLE "ToolTips in FiveWin! ���! ���! ���!!!"

   DEFINE BUTTONBAR oBar 3D SIZE 33, 33 OF oWnd

   DEFINE BUTTON OF oBar  FILE ".\bitmaps\Exit.bmp"            ;
      TOOLTIP "Exit �����"                                            ;
      ACTION oWnd:End()

   DEFINE BUTTON OF oBar  FILE ".\bitmaps\Plus.bmp"            ;
      TOOLTIP "Adding tooltips is... ������� tooltip...."                           ;
      ACTION MsgInfo("... as easy as this! .... ������������� �����!")

   @ 10, 8 BUTTON oBtn PROMPT "Where is my tooltip? ��� ��� tooltip?" SIZE 180, 28 OF oWnd ;
      ACTION MsgInfo( "Oh! Thanks! �! �������!" )

   // Manipulate directly the cToolTip DATA of other controls than ButtonBitmaps
   oBtn:cToolTip = "Here it is! � ��� ��!"


   SET MESSAGE OF oWnd                                          ;
      TO "FiveWin: Clip for Unix  && "+                    ;
	 "���-�� ��-������!!!"         ;
      CENTER							;
      DATE ;
      CLOCK ;
      KEYBOARD

   ACTIVATE WINDOW oWnd                                         ;

RETURN( NIL )

//-------------------------------------------------------------//
