function findOptions(str, rstr, color, colorhist, lcase, lwordonly, lregexp, lallw, ndirect, nwhere)
local obj
       obj:=map()
       obj:classname    := "FINDOPTIONS"
       obj:replace	:= .f.
       obj:fstring	:= iif (str!=NIL, str, "")
       obj:rstring	:= iif (rstr!=NIL, rstr, "")
       obj:colorSpec    := iif(empty(color),setcolor(),color)
       obj:colorHistSpec:= iif(empty(colorhist),setcolor(),colorhist)

       obj:case		:= iif(lcase!=NIL .and. valtype(lcase)=="L", lcase, .f.)
       obj:wordonly 	:= iif(lwordonly!=NIL .and. valtype(lwordonly)=="L", lwordonly, .f.)
       obj:regexp 	:= iif(lregexp!=NIL .and. valtype(lregexp)=="L", lregexp, .f.)
       obj:allw 	:= iif(lallw!=NIL .and. valtype(lallw)=="L", lallw, .f.)
       obj:direct 	:= iif(ndirect!=NIL .and. valtype(ndirect)=="N", ndirect, 1 )	// forward/backward/frombegin
       obj:where 	:= iif(nwhere!=NIL .and. valtype(nwhere)=="N", nwhere, 1 )	// in text/ in block

       obj:fhistory	:= historyObj(, , , , obj:colorHistSpec)
       obj:rhistory	:= historyObj(, , , , obj:colorHistSpec)

       obj:get		:= @get()
       obj:setfindstr	:= @setfindstr()
       obj:setreplstr	:= @setreplstr()
       obj:sethistfind	:= @sethistfind()
       obj:sethistrepl	:= @sethistrepl()
       obj:sethistorysize := @sethistorysize()

       obj:__colors 	:= {}      // ������� ������
       obj:__setColor 	:= @__setcolor()
       obj:__setcolor()
return obj
