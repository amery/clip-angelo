function listItemNew(Lrow, Lcol, Rrow, Rcol, Columns, Delimiter, color)
local obj
       obj:=map()

       obj:classname    := "LISTITEM"
       obj:first        := 1
       obj:colorSpec    := iif(color==NIL,setcolor(),color)
       obj:nTop         := Lrow
       obj:nLeft        := Lcol
       obj:nBottom      := Rrow-1
       obj:nRight       := Rcol
       obj:line         := Lrow
       obj:col		:= Rcol
       obj:pos          := 1
       obj:colWin       := iif(empty(Columns), 1, Columns)
       obj:rowWin       := Rrow-Lrow
       obj:lencol	:= {}
       obj:itemCount	:= 0
       obj:buffer	:= 0
       obj:hasFocus	:= .f.
       obj:title	:= {}
       obj:union	:= .f.
       obj:itemWin	:= obj:rowWin*obj:colWin
       obj:delim	:= iif(Delimiter==NIL, "|", Delimiter)
       obj:__keys	:= map()
       obj:nSelected	:= 0

       _recover_listitem(obj)

       obj:findbuffer  	:= ""

       obj:item 	:= {}
       obj:__colors	:= {}      // О©╫О©╫О©╫О©╫О©╫О©╫О©╫ О©╫О©╫О©╫О©╫О©╫О©╫

       obj:__init()
       obj:__setcolor()

       while (Rcol-Lcol)%obj:colWin > 0
	    Rcol --
       enddo
       asize(obj:lencol, obj:colWin)
       afill(obj:lencol, int((Rcol-Lcol)/obj:colWin))
return obj
