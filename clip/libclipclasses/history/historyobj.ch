function historyObj(Lrow, Lcol, Rrow, Rcol, color)
local obj
       obj:=map()
       obj:classname    := "HISTORYOBJECT"
       obj:Lrow         := iif(Lrow==NIL, 1, Lrow)
       obj:Lcol         := iif(Lcol==NIL, 1, Lcol)
       obj:Rrow         := iif(Rrow==NIL, maxrow(), Rrow)
       obj:Rcol         := iif(Rcol==NIL, maxcol(), Rcol)

       obj:history	:= {}
       obj:size		:= 0
       obj:header	:= ""

       obj:insert	:= @insert()
       obj:add		:= @add()
       obj:delete	:= @delete()
       obj:run		:= @run()
       obj:setsize	:= @setsize()
       obj:first	:= @first()
       obj:set		:= @set()

       obj:colorSpec    := iif(empty(color),setcolor(),color)
       obj:__colors 	:= {}      // ������� ������
       obj:__setColor 	:= @__setcolor()
       obj:__setcolor()
return obj
