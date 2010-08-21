
#include "dv_comment.ch"

Function dv_Line2Array (psLine , pbSpacedCommand )
//   Parameters psLine , pbSpacedCommand
     //
     //  dv_Line2Array("lsString  "| lsString_Var)
     //
     //  Return an array composed of the different lsWord from a lsString
     //
   Local lsWord := ""
   Local laReturnArray := {}
   Local ln01ii := 1
   Local ln02jj := 1
   Local ln03kk := 1
   Local lsString := ""
   Local lnLengthLine := 0
   Local lcLetter := ""
   Local lsDbf := ""
   Local lsAllLetters := "0123456789_ABCDEFGHIJKLMNOPQRSTUVWXYZ"
   lsDbf := Select ( )
   Default pbSpacedCommand To .T.
   lsString := AllTrim ( psLine ) + Space ( 1 )
   lnLengthLine := Len ( lsString )
   Do Case
   Case lnLengthLine = 0
      Return laReturnArray
   Case SubStr ( lsString , 1 , 1 ) == "#"
      lsWord := "#"
      lcLetter := SubStr ( lsString , ++ ln01ii , 1 )
      Do While Upper ( lcLetter ) $ lsAllLetters
         lsWord := lsWord + lcLetter
         lcLetter := SubStr ( lsString , ++ ln01ii , 1 )
         If ln01ii > lnLengthLine
            Exit
         EndIf
      EndDo
      aAdd ( laReturnArray , lsWord )
   Case SubStr ( lsString , 1 , 1 ) == "*"
      lsWord := lsString
      ln01ii := lnLengthLine + 1
*   Case SubStr ( lsString , 1 , 2 ) == COMMENT_BEGIN
*      lsWord := lsString
*      ln01ii := lnLengthLine + 1
   Case SubStr ( lsString , 1 , 2 ) == COMMENT_SLASH //
      lsWord := lsString
      ln01ii := lnLengthLine + 1
   Case SubStr ( lsString , 1 , 2 ) == COMMENT_AMP &&
      lsWord := lsString
      ln01ii := lnLengthLine + 1
   Case Upper ( SubStr ( lsString , 1 , 5 ) ) == "NOTE "
      lsWord := lsString
      ln01ii := lnLengthLine + 1
   EndCase
   Do While ln01ii <= lnLengthLine
      lsWord := Space ( 0 )
      Do While ( ln01ii <= lnLengthLine )
         lcLetter := SubStr ( lsString , ln01ii ++ , 1 )
         Do Case
         Case Upper ( SubStr ( lsString , ln01ii - 1 , 3 ) ) == ".T."
            lsWord := ".T."
            ln01ii := ln01ii + 2
            Exit
         Case Upper ( SubStr ( lsString , ln01ii - 1 , 3 ) ) == ".F."
            lsWord := ".F."
            ln01ii := ln01ii + 2
            Exit
         Case Upper ( SubStr ( lsString , ln01ii - 1 , 5 ) ) == ".AND."
            lsWord := ".AND."
            ln01ii := ln01ii + 4
            Exit
         Case Upper ( SubStr ( lsString , ln01ii - 1 , 4 ) ) == ".OR."
            lsWord := ".OR."
            ln01ii := ln01ii + 3
            Exit
         Case Upper ( SubStr ( lsString , ln01ii - 1 , 5 ) ) == ".NOT."
            lsWord := ".NOT."
            ln01ii := ln01ii + 4
            Exit
         Case ( lcLetter == ":" ) .And. SubStr ( lsString , ln01ii , 1 ) == "="
            If ( .Not. Empty ( lsWord ) )
               -- ln01ii
               Exit
            EndIf
            lsWord := lsWord + lcLetter + SubStr ( lsString , ln01ii ++ , 1 )
            Exit
         Case ( lcLetter == "+" ) .And. SubStr ( lsString , ln01ii , 1 ) == "="
            If ( .Not. Empty ( lsWord ) )
               -- ln01ii
               Exit
            EndIf
            lsWord := lsWord + lcLetter + SubStr ( lsString , ln01ii ++ , 1 )
            Exit
         Case ( lcLetter == "-" ) .And. SubStr ( lsString , ln01ii , 1 ) == "="
            If ( .Not. Empty ( lsWord ) )
               -- ln01ii
               Exit
            EndIf
            lsWord := lsWord + lcLetter + SubStr ( lsString , ln01ii ++ , 1 )
            Exit
         Case ( lcLetter == "<" ) .And. SubStr ( lsString , ln01ii , 1 ) == ">"
            If ( .Not. Empty ( lsWord ) )
               -- ln01ii
               Exit
            EndIf
            lsWord := lsWord + lcLetter + SubStr ( lsString , ln01ii ++ , 1 )
            Exit
         Case ( lcLetter == "<" ) .And. SubStr ( lsString , ln01ii , 1 ) == "="
            If ( .Not. Empty ( lsWord ) )
               -- ln01ii
               Exit
            EndIf
            lsWord := lsWord + lcLetter + SubStr ( lsString , ln01ii ++ , 1 )
            Exit
         Case ( lcLetter == Chr ( 9 ) )
            If ( .Not. Empty ( lsWord ) )
               Exit
            EndIf
            Loop
         Case ( lcLetter == ">" ) .And. SubStr ( lsString , ln01ii , 1 ) == "="
            If ( .Not. Empty ( lsWord ) )
               -- ln01ii
               Exit
            EndIf
            lsWord := lsWord + lcLetter + SubStr ( lsString , ln01ii ++ , 1 )
            Exit
         Case ( lcLetter == "=" ) .And. SubStr ( lsString , ln01ii , 1 ) == ">"
           	lsWord := lsWord + lcLetter + SubStr ( lsString , ln01ii ++ , 1 )
            Exit
         Case ( lcLetter == "?" ) .And. SubStr ( lsString , ln01ii , 1 ) == "?"
            If ( .Not. Empty ( lsWord ) )
               -- ln01ii
               Exit
            EndIf
            lsWord := lsWord + lcLetter + SubStr ( lsString , ln01ii ++ , 1 )
            Exit
         Case ( lcLetter == "?" )
            If ( .Not. Empty ( lsWord ) )
               -- ln01ii
               Exit
            EndIf
            lsWord := lsWord + lcLetter + SubStr ( lsString , ln01ii ++ , 1 )
            Exit
         Case ( lcLetter == "!" ) .And. SubStr ( lsString , ln01ii , 1 ) == "="
            If ( .Not. Empty ( lsWord ) )
               -- ln01ii
               Exit
            EndIf
            lsWord := lsWord + lcLetter + SubStr ( lsString , ln01ii ++ , 1 )
            Exit
         Case ( lcLetter == "*" ) .And. SubStr ( lsString , ln01ii , 1 ) == "="
            If ( .Not. Empty ( lsWord ) )
               -- ln01ii
               Exit
            EndIf
            lsWord := lsWord + lcLetter + SubStr ( lsString , ln01ii ++ , 1 )
            Exit
         Case ( lcLetter == "*" ) .And. SubStr ( lsString , ln01ii , 1 ) == "*"
            If ( .Not. Empty ( lsWord ) )
               -- ln01ii
               Exit
            EndIf
            lsWord := lsWord + lcLetter + SubStr ( lsString , ln01ii ++ , 1 )
            Exit
         Case ( lcLetter == "%" ) .And. SubStr ( lsString , ln01ii , 1 ) == "="
            If ( .Not. Empty ( lsWord ) )
               -- ln01ii
               Exit
            EndIf
            lsWord := lsWord + lcLetter + SubStr ( lsString , ln01ii ++ , 1 )
            Exit
         Case ( lcLetter == "&" ) .And. SubStr ( lsString , ln01ii , 1 ) == "&"
            If ( .Not. Empty ( lsWord ) )
               -- ln01ii
               Exit
            EndIf
            If Len ( laReturnArray ) > 0
               lsWord := Chr ( 13 ) + Chr ( 10 ) + lsWord + lcLetter + SubStr ( lsString , ln01ii )
            Else
               lsWord := lsWord + lcLetter + SubStr ( lsString , ln01ii )
            EndIf
            ln01ii := lnLengthLine + 1
         Case ( lcLetter == "@" ) .And. ( .Not. Empty ( lsWord ) )
            -- ln01ii
            Exit
         Case ( lcLetter == "@" ) .And. ( Empty ( lsWord ) )
            lsWord := lsWord + lcLetter
            Exit
         Case ( lcLetter == "{" ) .And. ( .Not. Empty ( lsWord ) )
            -- ln01ii
            Exit
         Case ( lcLetter == "}" ) .And. ( .Not. Empty ( lsWord ) )
            -- ln01ii
            Exit
         Case ( lcLetter == "[" ) .And. ( .Not. Empty ( lsWord ) ) .And. pbSpacedCommand
            -- ln01ii
            Exit
         Case ( lcLetter == "[" ) .And. ( .Not. Empty ( lsWord ) )
            lsWord := lsWord + lcLetter
            Exit
         Case ( lcLetter == "]" ) .And. ( .Not. Empty ( lsWord ) )
            -- ln01ii
            Exit
         Case ( lcLetter == "#" ) .And. ( .Not. Empty ( lsWord ) )
            -- ln01ii
            Exit
         Case ( lcLetter == "^" ) .And. ( .Not. Empty ( lsWord ) )
            -- ln01ii
            Exit
         Case ( lcLetter == ")" ) .And. ( .Not. Empty ( lsWord ) )
            -- ln01ii
            Exit
         Case ( lcLetter == ";" ) .And. ( .Not. Empty ( lsWord ) )
            -- ln01ii
            Exit
         Case ( lcLetter == "-" ) .And. SubStr ( lsString , ln01ii , 1 ) == ">"
            lsWord := lsWord + lcLetter + SubStr ( lsString , ln01ii ++ , 1 )
         Case ( lcLetter == "&" ) .And. SubStr ( lsString , ln01ii , 1 ) == "&"
            If ( .Not. Empty ( lsWord ) )
               -- ln01ii
               Exit
            EndIf
            lsWord := lsWord + lcLetter + SubStr ( lsString , ln01ii )
            ln01ii := lnLengthLine + 1
         Case ( lcLetter == "/" ) .And. SubStr ( lsString , ln01ii , 1 ) == "/"
            If ( .Not. Empty ( lsWord ) )
               -- ln01ii
               Exit
            EndIf
            If Len ( laReturnArray ) > 0
               lsWord := Chr ( 13 ) + Chr ( 10 ) + lsWord + lcLetter + SubStr ( lsString , ln01ii )
            Else
               lsWord := lsWord + lcLetter + SubStr ( lsString , ln01ii )
            EndIf
            ln01ii := lnLengthLine + 1
         Case ( lcLetter == "/" ) .And. SubStr ( lsString , ln01ii , 1 ) == "*"
            If ( .Not. Empty ( lsWord ) )
               -- ln01ii
               Exit
            EndIf
            If Len ( laReturnArray ) > 0
               lsWord := Chr ( 13 ) + Chr ( 10 ) + lsWord + lcLetter + SubStr ( lsString , ln01ii )
            Else
               lsWord := lsWord + lcLetter + SubStr ( lsString , ln01ii )
            EndIf
            ln01ii := lnLengthLine + 1
         Case ( lcLetter == "-" ) .And. SubStr ( lsString , ln01ii , 1 ) == "-"
            If ( .Not. Empty ( lsWord ) )
               -- ln01ii
               Exit
            EndIf
            lsWord := lsWord + lcLetter + SubStr ( lsString , ln01ii ++ , 1 )
            Exit
         Case ( lcLetter == "-" ) .And. SubStr ( lsString , ln01ii , 1 ) == "="
            If ( .Not. Empty ( lsWord ) )
               -- ln01ii
               Exit
            EndIf
            lsWord := lsWord + lcLetter + SubStr ( lsString , ln01ii ++ , 1 )
            Exit
         Case ( lcLetter == "+" ) .And. SubStr ( lsString , ln01ii , 1 ) == "+"
            If ( .Not. Empty ( lsWord ) )
               -- ln01ii
               Exit
            EndIf
            lsWord := lsWord + lcLetter + SubStr ( lsString , ln01ii ++ , 1 )
            Exit
         Case ( lcLetter == "+" ) .And. SubStr ( lsString , ln01ii , 1 ) == "="
            If ( .Not. Empty ( lsWord ) )
               -- ln01ii
               Exit
            EndIf
            lsWord := lsWord + lcLetter + SubStr ( lsString , ln01ii ++ , 1 )
            Exit
         Case ( lcLetter == ":" ) .And. SubStr ( lsString , ln01ii , 1 ) == "="
            If ( .Not. Empty ( lsWord ) )
               -- ln01ii
               Exit
            EndIf
            lsWord := lsWord + lcLetter + SubStr ( lsString , ln01ii ++ , 1 )
            Exit
         Case ( lcLetter == "=" ) .And. SubStr ( lsString , ln01ii , 1 ) == "="
            If ( .Not. Empty ( lsWord ) )
               -- ln01ii
               Exit
            EndIf
            lsWord := lsWord + lcLetter + SubStr ( lsString , ln01ii ++ , 1 )
            Exit
         Case ( lcLetter == "+" ) .And. ( .Not. Empty ( lsWord ) )
            -- ln01ii
            Exit
         Case ( lcLetter == "=" ) .And. ( .Not. Empty ( lsWord ) )
            -- ln01ii
            Exit
         Case ( lcLetter == "-" ) .And. ( .Not. Empty ( lsWord ) )
            -- ln01ii
            Exit
         Case ( ( lcLetter == "&" ) .And. Upper ( SubStr ( lsString , ln01ii , 1 ) ) $ (lsAllLetters + ".") ) .Or. Upper ( lcLetter ) $ (lsAllLetters + ".")
              //          Case Upper ( lcLetter ) $ "0123456789_ABCDEFGHIJKLMNOPQRSTUVWXYZ.:\&"
            lsWord := lsWord + lcLetter
            lcLetter := SubStr ( lsString , ln01ii ++ , 1 )
            Do While Upper ( lcLetter ) $ lsAllLetters
               lsWord := lsWord + lcLetter
               lcLetter := SubStr ( lsString , ln01ii ++ , 1 )
               If ln01ii > lnLengthLine
                  Exit
               EndIf
            EndDo
            -- ln01ii
         Case lcLetter == ":" .And. ( .Not. Empty ( lsWord ) )
            -- ln01ii
            Exit
         Case lcLetter == ":"
            lsWord := lsWord + lcLetter
            Exit
         Case lcLetter == ":" .And. Upper ( SubStr ( lsString , ln01ii , 1 ) ) $ "0123456789_ABCDEFGHIJKLMNOPQRSTUVWXYZ.:\"
            lsWord := lsWord + lcLetter
         Case lcLetter == "," .And. ( .Not. Empty ( lsWord ) )
            -- ln01ii
            Exit
         Case lcLetter == ","
            lsWord := lsWord + lcLetter
            Exit
         Case ( lcLetter == "(" ) .And. ( .Not. Empty ( lsWord ) )
            -- ln01ii
            Exit
         Case ( lcLetter == "(" ) .And. ( Empty ( lsWord ) )
            lsWord := lsWord + lcLetter
            Exit
         Case ( lcLetter == Space ( 1 ) ) .And. ( .Not. Empty ( lsWord ) )
            Exit
         Case ( lcLetter == "<" ) .And. ( .Not. Empty ( lsWord ) )
            -- ln01ii
            Exit
         Case ( lcLetter == ">" ) .And. ( .Not. Empty ( lsWord ) )
            -- ln01ii
            Exit
         Case ( lcLetter == "." ) .And. ( .Not. Empty ( lsWord ) )
            -- ln01ii
            Exit
         Case ( lcLetter == ":" ) .And. ( .Not. Empty ( lsWord ) )
            -- ln01ii
            Exit
         Case ( lcLetter == "$" ) .And. ( .Not. Empty ( lsWord ) )
            -- ln01ii
            Exit
         Case ( lcLetter == "=" ) .And. ( .Not. Empty ( lsWord ) )
            -- ln01ii
            Exit
         Case ( lcLetter == "%" ) .And. ( .Not. Empty ( lsWord ) )
            -- ln01ii
            Exit
         Case ( lcLetter == "*" ) .And. ( .Not. Empty ( lsWord ) )
            -- ln01ii
            Exit
         Case ( lcLetter == "&" ) .And. ( .Not. Empty ( lsWord ) )
            -- ln01ii
            Exit
         Case ( lcLetter == "!" ) .And. ( .Not. Empty ( lsWord ) )
            -- ln01ii
            Exit
         Case ( lcLetter == "/" ) .And. ( .Not. Empty ( lsWord ) )
            -- ln01ii
            Exit
         Case ( lcLetter == '"' ) .And. ( .Not. Empty ( lsWord ) )
            -- ln01ii
            Exit
         Case ( lcLetter == "'" ) .And. ( .Not. Empty ( lsWord ) )
            -- ln01ii
            Exit
         Case ( lcLetter == Space ( 1 ) ) .And. ( Empty ( lsWord ) )
            Loop
         Case ( lcLetter == "'" )
            lcLetter := SubStr ( lsString , ln01ii ++ , 1 )
            Do While lcLetter # "'"
               lsWord := lsWord + lcLetter
               lcLetter := SubStr ( lsString , ln01ii ++ , 1 )
               If ln01ii > lnLengthLine
                  Exit
               EndIf
            EndDo
           If '"' $ lsWord
               lsWord := "'" + lsWord + "'"
            Else
               lsWord := '"' + lsWord + '"'
            EndIf
            Exit
         Case ( lcLetter == '"' )
            lcLetter := SubStr ( lsString , ln01ii ++ , 1 )
            Do While lcLetter # '"'
               lsWord := lsWord + lcLetter
               lcLetter := SubStr ( lsString , ln01ii ++ , 1 )
               If ln01ii > lnLengthLine
                  Exit
               EndIf
            EndDo
            lsWord := '"' + lsWord + '"'
            Exit
         Case ( lcLetter == '<' )
            lcLetter := SubStr ( lsString , ln01ii ++ , 1 )
            Do While lcLetter # '>'
               lsWord := lsWord + lcLetter
               lcLetter := SubStr ( lsString , ln01ii ++ , 1 )
               If ln01ii > lnLengthLine
                  Exit
               EndIf
            EndDo
            lsWord := '<' + lsWord + '>'
            Exit
         Otherwise
            If ( .Not. Empty ( lsWord ) )
               -- ln01ii
               Exit
            EndIf
            lsWord := lsWord + lcLetter
            Exit
         EndCase
      EndDo
      If Str ( Val ( lsWord ) ) == lsWord
      	aAdd ( laReturnArray , Val( lsWord ) )
		Else
      	aAdd ( laReturnArray , lsWord )
		EndIf
      lsWord := Space ( 0 )
   EndDo
   aAdd ( laReturnArray , lsWord )
   Return laReturnArray


