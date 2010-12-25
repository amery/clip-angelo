function _listbox_(top,left,bottom,right,var,items,caption,message,;
		   color, fblock,sblock,ldrop,scroll,bitmap)

   local i,dtext,data,obj
   obj		:= listbox(top,left,bottom,right,ldrop)
   obj:caption	:= iif(caption==NIL,"",caption)
   obj:message	:= message
   obj:bitmap	:= bitmap
   obj:fblock	:= fblock
   obj:sblock	:= sblock

   obj:__bufType:=2
   for i=1 to len(items)
       do case
       case valtype(items[i])=="C"
	    dtext := items[i]
	    obj:__bufType:=1
       case valtype(items[i])=="A"
	    dtext := toString(items[i][1])
	    if len(items[i])>1
		data := items[i][2]
	    else
		obj:__bufType:=1
	    endif
       otherwise
	    dtext := toString(items[i])
       endcase
       obj:addItem(dtext,data,.f.)
       if obj:__bufType==1 .and. valtype(dtext)==valtype(var) .and. dtext==var
	  obj:value:=i
       endif
       if obj:__bufType==2 .and. valtype(data)==valtype(var) .and. data==var
	  obj:value:=i
       endif
   next
   if valtype(var)=="N"
      obj:__bufType:=0
      obj:value:=var
   endif
   obj:value:=min(obj:itemCount,max(obj:value,1))
   obj:__line:=obj:value
   obj:colorSpec:=iif(color==NIL,setcolor(),color)
   if scroll!=NIL .and. scroll
      obj:__SB	:= scrollBar(top+iif(obj:dropDown,1,0),bottom,right,sblock)
      obj:__SB:colorSpec:=obj:colorSpec
   endif

   obj:__setColor()
   obj:__setBuffer()
   obj:display()

return obj
