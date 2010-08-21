function _radiogrp_(top,left,bottom,right,var,buttons,caption,message,color,;
		    fblock,style)
   local i,obj
   obj:=radioGroup(top,left,bottom,right)
   obj:message	:=message
   obj:caption	:=caption
   obj:fblock	:=fblock
   for i=1 to len(buttons)
       obj:addItem(buttons[i])
   next
   obj:__setvalue(1)
   if style!=NIL
      obj:setStyle(style)
   endif
   if valtype(var)=="N"
      	obj:__setvalue(var)
   else
        //obj:value:=iif(obj:typeOut,1,0)
	for i=1 to len(obj:__items)
               	if valtype(obj:__items[i]:rvalue) == valtype(var) ;
        	               	.and. obj:__items[i]:rvalue == var

			obj:__setValue(i)
                endif

	next
   endif
   if obj:value!=0
      obj:__items[obj:value]:buffer:=.t.
      obj:__items[obj:value]:setFocus()
   endif

   obj:colorSpec:=iif(empty(color),setcolor(),color)
   obj:__setcolor()
   obj:display(.f.)
return obj
