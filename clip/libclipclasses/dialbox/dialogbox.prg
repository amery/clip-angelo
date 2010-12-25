function dialogBox(Lrow, Lcol, Rrow, Rcol, color)
local obj
       obj:=map()
       obj:classname    := "DIALOGBOX"

       obj:lines        := 0
       obj:colorSpec    := iif(color==NIL .or. empty(color),DBCOLORS,color)
       obj:line         := 1
       obj:pos          := 1
       obj:colWin       := 1
       obj:rowWin       := 1
       obj:nT        	:= Lrow
       obj:nL        	:= Lcol
       obj:nB      	:= Rrow
       obj:nR       	:= Rcol
       obj:nTop         := obj:nT+1
       obj:nLeft        := obj:nL+1
       obj:nBottom      := obj:nB-1
       obj:nRight       := obj:nR-1
       obj:marginLeft	:= 8
       obj:header	:= ""
       obj:Find		:= 0
       obj:n_Line	:= 1
       obj:n_pos	:= 1
       obj:leninfo	:= 0

       obj:RadioItem	:= map()

       obj:regSearch	:= 0
       obj:findR	:= {}

       obj:mapKeys	:= HK_get("dialog")
       obj:inbuffer	:= {}

       obj:__nbr	:= 0
       obj:item		:= map()
       obj:item_type	:= 0
       obj:item_area	:= {}

       obj:__oldscr	:= 0
       obj:__oldcolr	:= 0
       obj:__oldcursor	:= 0
       obj:__colors 	:= {}      // ������� ������

       _recover_dialogbox(obj)

       obj:__setcolor()
       obj:init()
return obj
