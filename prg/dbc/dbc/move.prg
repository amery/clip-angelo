/*
 * DBC - Database Control Utility
 * DB move functions
 *
 * Author: Alexander S.Kresin <alex@belacy.belgorod.su>
 *         www - http://www.geocities.com/alkresin/
 * Released to Public Domain
*/
*+��������������������������������������������������������������������
*+
*+    Source Module => D:\MYAPPS\SOURCE\MYDBU\MOVE.PRG
*+
*+    Functions: Function F_LOCATE()
*+               Function F_CONTIN()
*+               Function F_FILTER()
*+               Function F_SEEK()
*+               Function F_GO()
*+
*+    Reformatted by Click! 2.00 on Jun-20-2001 at 12:52 pm
*+
*+��������������������������������������������������������������������

#include "ci_deflist.ch"

*+��������������������������������������������������������������������
*+
*+    Function F_LOCATE()
*+
*+��������������������������������������������������������������������
*+

Static cLocate := "", cFilter := "", cSeek := ""
#ifdef ENGLISH
Static cMsg1 := "Record not found.", cMsg2 := "Done!", cMsg3 := "Wrong expression."
#else
Static cMsg1 := "������ �� �������.", cMsg2 := "��⮢�!", cMsg3 := "����୮� ��ࠦ����."
#endif

FUNCTION F_LOCATE

LOCAL nrec, i, block, res := .F.
#ifdef ENGLISH
LOCAL cMsg1 := "Record not found.", cMsg2 := "Done!", cMsg3 := "Wrong expression."
   cLocate := MsgGet( "Search expression:",,cLocate )
#else
LOCAL cMsg1 := "������ �� �������.", cMsg2 := "��⮢�!", cMsg3 := "����୮� ��ࠦ����."
   cLocate := MsgGet( "������� ��ப� ��� ���᪠",,cLocate )
#endif
   IF .NOT. EMPTY( cLocate )
      IF VALTYPE( &cLocate ) == "L"
         nrec := RECNO()
         MsgInf( "����� ..." )
         block := &( "{||" + Iif( lWinChar, DosToWin( cLocate ), cLocate ) + "}" )
         IF LI_PRFLT
            FOR i := 1 TO Min( LI_KOLZ,m->klrecf-1 )
               GO LI_MSREC[ i ]
               IF Eval( block )
                  res := .T.
                  EXIT
               ENDIF
            NEXT
            IF !res .AND. i < LI_KOLZ
               SKIP
               DO WHILE !Eof()
                  IF Eval( block )
                     res := .T.
                     EXIT
                  ENDIF
                  i ++
                  SKIP
               ENDDO
            ENDIF
         ELSE
            __dbLocate( block,,,, .F. )
         ENDIF
         IF ( LI_PRFLT .AND. !res ) .OR. ( !LI_PRFLT .AND. .NOT. FOUND() )
            GO nrec
            MsgInf(cMsg1 )
         ELSE
            Msginf( cMsg2 )
            IF LI_PRFLT
               LI_TEKZP := i
            ENDIF
         ENDIF
      ELSE
         MsgSay( cMsg3 )
      ENDIF
   ENDIF
RETURN 0

*+��������������������������������������������������������������������
*+
*+    Function F_CONTIN()
*+
*+��������������������������������������������������������������������
*+
FUNCTION F_CONTIN

LOCAL nrec, i, block, res := .F.
#ifdef ENGLISH
   MsgInf( "Wait ..." )
#else
   MsgInf( "����� ..." )
#endif
   nrec := RECNO()
   IF LI_PRFLT
      block := &( "{||" + Iif( lWinChar, DosToWin( cLocate ), cLocate ) + "}" )
      FOR i := LI_TEKZP+1 TO Min( LI_KOLZ,m->klrecf-1 )
         GO LI_MSREC[ i ]
         IF Eval( block )
            res := .T.
            EXIT
         ENDIF
      NEXT
      IF !res .AND. i < LI_KOLZ
         SKIP
         DO WHILE !Eof()
            IF Eval( block )
               res := .T.
               EXIT
            ENDIF
            i ++
            SKIP
         ENDDO
      ENDIF
   ELSE
      CONTINUE
   ENDIF
   IF ( LI_PRFLT .AND. !res ) .OR. ( !LI_PRFLT .AND. .NOT. FOUND() )
      GO nrec
      MsgInf( cMsg1 )
   ELSE
      Msginf( cMsg2 )
      IF LI_PRFLT
         LI_TEKZP := i
      ENDIF
   ENDIF
RETURN Nil

*+��������������������������������������������������������������������
*+
*+    Function F_FILTER()
*+
*+��������������������������������������������������������������������
*+
FUNCTION F_FILTER( strfnd )

LOCAL nrec, i
   IF strfnd == Nil
#ifdef ENGLISH
      cFilter := MsgGet( "Input filter expression:",,cFilter )
#else
      cFilter := MsgGet( "������� ��ࠦ���� ��� 䨫����:",,cFilter )
#endif
   ELSE
     cFilter := strfnd
   ENDIF
   IF .NOT. EMPTY( cFilter )
      IF VALTYPE( &cFilter ) == "L"
         nrec := RECNO()
#ifdef ENGLISH
         MsgInf( "Wait ..." )
#else
         MsgInf( "����� ..." )
#endif
         IF lWinChar
            dbSetFilter( &( "{||"+ DosToWin( cFilter ) + "}" ), DosToWin( cFilter ) )
         ELSE
            dbSetFilter( &( "{||"+ cFilter + "}" ), cFilter )
         ENDIF
         GO TOP
         i       := 1
         LI_KOLZ := 0
         IF LI_MSREC == Nil
            LI_MSREC := Array( m->klrecf )
         ENDIF
         DO WHILE .NOT. EOF()
            LI_MSREC[ i ] = RECNO()
            IF i < m->klrecf
               i ++
            ENDIF
            LI_KOLZ ++
            IF INKEY() = 27
               LI_KOLZ := 0
               EXIT
            ENDIF
            SKIP
         ENDDO
         LI_TEKZP := 1
         IF LI_KOLZ > 0
            GO LI_MSREC[ 1 ]
            LI_PRFLT := .T.
            MsgInf( cMsg2 )
         ELSE
            LI_PRFLT := .F.
            SET FILTER TO
            GO nrec
            MsgInf( cMsg1 )
         ENDIF
      ELSE
         MsgSay( cMsg3 )
      ENDIF
   ELSEIF LASTKEY() <> 27
      LI_PRFLT := .F.
      SET FILTER TO
#ifdef ENGLISH
      MsgInf( "Filter off" )
#else
      MsgInf( "������ �⪫�祭" )
#endif
   ENDIF
RETURN Nil

*+��������������������������������������������������������������������
*+
*+    Function F_SEEK()
*+
*+��������������������������������������������������������������������
*+
FUNCTION F_SEEK
LOCAL key

#ifdef ENGLISH
   cSeek := MsgGet( "Input key value",,cSeek )
#else
   cSeek := MsgGet( "������� ���祭�� �����",,cSeek )
#endif
   IF .NOT. EMPTY( cSeek )
      m->nrec := RECNO()
      IF TYPE( ORDKEY() ) = "N"
         key := VAL( cSeek )
      ELSEIF TYPE( ORDKEY() ) = "D"
         key := CTOD( Trim( cSeek ) )
      ELSEIF lWinChar
         key := DosToWin( cSeek )
      ELSE
         key := cSeek
      ENDIF
      SEEK key
      IF .NOT. FOUND()
         GO m->nrec
         MsgSay( cMsg1 )
      ENDIF
   ENDIF
RETURN Nil

*+��������������������������������������������������������������������
*+
*+    Function F_GO()
*+
*+��������������������������������������������������������������������
*+
FUNCTION F_GO

#ifdef ENGLISH
   m->strfnd := MsgGet( "Record number", "999999999" )
#else
   m->strfnd := MsgGet( "����� ������", "999999999" )
#endif
   IF ( m->nrec := VAL( m->strfnd ) ) <> 0
      GO m->nrec
   ENDIF
RETURN Nil

*+ EOF: MOVE.PRG
