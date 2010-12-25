function textEditNew(Lrow, Lcol, Rrow, Rcol, color)
local obj
       obj:=map()

       obj:classname    := "TEXTEDIT"
       obj:path         := ""  // ���� � �����
       obj:filename     := ""  // ��� �����
       obj:lines        := 0
       obj:colorSpec    := iif(empty(color),setcolor(),color)
       obj:line         := 1
       obj:pos          := 1
       obj:colWin       := 1
       obj:rowWin       := 1
       obj:nTop         := Lrow
       obj:nLeft        := Lcol
       obj:nBottom      := Rrow
       obj:nRight       := Rcol
       obj:updated      := .f.
       obj:marginLeft   := TE_MARGIN_LEFT
       obj:marginRight  := TE_MARGIN_RIGHT
       obj:tabSize      := TE_TABSIZE
       obj:maxStrings   := TE_MAXSTRINGS
       obj:Hyphen       := TE_HYPHEN
       obj:tabPack      := iif(lower(set("edit_tabpack"))=='yes',.t.,.f.)
       obj:inFocus      := .f.
       obj:autoWrap     := TE_AUTO_WRAP

       obj:mkblock      := .f.
       obj:strblock     := .f.          // �������� ����
       obj:rectblock    := .f.          // ������������� ����
       obj:koordblock   := {NIL, NIL, NIL, NIL} // ���������� �����

       obj:__findR      := {}
       obj:__regSearch  := {}
       obj:__undobuffer := {}
       obj:lenundo      := 100
       obj:__curundo    := 0
       obj:__startundo  := 0
       obj:charset      := NIL
       obj:lEofString   := .f.      // show eof string
       obj:eofString    := [<EOF>]
       obj:__hostcharset:= host_charset()
       obj:__keys       := map()

       obj:highLightColor       := map()

       obj:Nstyle       := .f.
       obj:__LNstyle    := 0

       obj:edbuffer     := {}
       obj:__colors     := {}      // ������� ������
       obj:__leninfo    := maxcol()-maxcol()*3/4 - 1

       _recover_textedit(obj)

       obj:__setcolor()
       obj:__setDefaultKey()
       obj:__nullInit()

return obj
