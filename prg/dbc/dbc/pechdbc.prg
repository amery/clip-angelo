/*
 * DBC - Database Control Utility
 * Printing
 *
 * Author: Alexander S.Kresin <alex@belacy.belgorod.su>
 *         www - http://www.geocities.com/alkresin/
 * Released to Public Domain
*/
*+膊膊膊膊膊膊膊膊膊膊膊膊膊膊膊膊膊膊膊膊膊膊膊膊膊膊膊膊膊膊膊膊膊膊
*+
*+    Source Module => D:\MYAPPS\SOURCE\MYDBU\PECHDBC.PRG
*+
*+    Functions: Function PECH()
*+               Static Function Setpch()
*+               Static Function EndPch()
*+               Function PrnStru1()
*+
*+    Reformatted by Click! 2.00 on Jun-20-2001 at 12:51 pm
*+
*+膊膊膊膊膊膊膊膊膊膊膊膊膊膊膊膊膊膊膊膊膊膊膊膊膊膊膊膊膊膊膊膊膊膊

#include "deflist.ch"
STATIC bufekr

*+北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北
*+
*+    Function PECH()
*+
*+北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北
*+
FUNCTION PECH

LOCAL fif, x, sviv, i, kolfld
PRIVATE prmsf, klfs

   IF !SetPch()
      Return .F.
   ENDIF
   // 彞鐮忪
   IF LI_MSF == Nil
      prmsf  := .F.
      kolfld := FCOUNT()
   ELSE
      prmsf := .T.
      FOR i := 1 TO LEN( LI_MSF )
         IF LI_MSF[ i ] = Nil
            EXIT
         ENDIF
      NEXT
      kolfld := i - 1
   ENDIF
   klfs := kolfld
   FGOTOP( mslist )
   DO WHILE .NOT. FEOF( mslist )
      IF .NOT. DELETED()
         fif := 1
         ?
         FOR i := 1 TO kolfld
            sviv := FLDSTR( mslist, fif )
            ?? sviv + ' '
            fif ++
         NEXT
      ENDIF
      FSKIP( mslist, 1 )
   ENDDO
   Endpch()
RETURN .T.

*+北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北
*+
*+    Static Function Setpch()
*+
*+    Called from ( pechdbc.prg  )   1 - function pech()
*+                                   1 - function prnstru1()
*+
*+北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北
*+
STATIC FUNCTION Setpch

LOCAL lkey, choic, fname
#ifdef ENGLISH
LOCAL str1 := " TO PRINTER ", str2 := " TO FILE    "
LOCAL str3 := "PRINTER NOT READY", str4 := "Prepare it and press ENTER", str5 := "Esc - Exit"
LOCAL str6 := "Output file name:"
#else
LOCAL str1 := " 崁 彁垗拝� ", str2 := " � 攢墜     "
LOCAL str3 := "彁圚拝� H� 儙拵�!", str4 := "彯ぃ焘� � Μㄢ� ENTER", str5 := "Esc - 傠瀹�"
LOCAL str6 := "埇� ㈦瀹き� 錉┇�:"
#endif
   bufekr := SAVESCREEN( 9, 32, 13, 60 )
   SET COLOR TO +GR/B,N/BG,,,+GR/B
   @  9, 32, 13, 60 BOX ORAMKA
   choic := 1
   @ 10, 34 PROMPT str1
   @ 11, 34 PROMPT str2
   MENU TO choic
   IF choic = 1
#ifndef __NOEXTRA__
      IF .NOT. PRINTREADY( 1 )
         DO WHILE .T.
            @  9, 32, 13, 60 BOX ORAMKA
            @ 10, 34 SAY str3         
            @ 11, 34 SAY str4         
            @ 12, 34 SAY str5         
            lkey := INKEY( 0 )
            IF lkey = 13
               IF PRINTREADY( 1 )
                  EXIT
               ENDIF
               @  9, 32, 13, 60 BOX ORAMKA
               INKEY( 1 )
            ELSEIF lkey = 27
               RESTSCREEN( 9, 32, 13, 60, bufekr )
               RETURN .F.
            ENDIF
         ENDDO
      ENDIF
#endif
      fname := "LPT1"
   ELSEIF choic = 0
      RESTSCREEN( 9, 32, 13, 60, bufekr )
      RETURN .F.
   ELSE
      IF EMPTY( fname := MsgGet( str6 ) )
         RETURN .F.
      ENDIF
   ENDIF
   SET ALTERNATE TO &fname
   SET ALTERNATE ON
   SET CONSOLE OFF
RETURN .T.

*+北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北
*+
*+    Static Function EndPch()
*+
*+    Called from ( pechdbc.prg  )   1 - function pech()
*+                                   1 - function prnstru1()
*+
*+北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北
*+
STATIC FUNCTION EndPch

   SET CONSOLE ON
   SET ALTERNATE OFF
   CLOSE ALTERNATE
   RESTSCREEN( 9, 32, 13, 60, bufekr )
RETURN Nil

*+北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北
*+
*+    Function PrnStru1()
*+
*+北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北
*+
FUNCTION PrnStru1

LOCAL i

   IF LASTKEY() == 281
      Setpch()
      ? msfile[ improc ] + ":"
      ? "--------------"
      FOR i := 1 TO LEN( m1 )
         ? m1[ i ]
      NEXT
      Endpch()
   ENDIF
RETURN 2

*+ EOF: PECHDBC.PRG
