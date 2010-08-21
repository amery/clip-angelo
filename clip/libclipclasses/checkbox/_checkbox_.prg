function _checkbox_(var,caption,message,color,fblock,sblock,style,bitmaps,row,col)
   local obj
   iif(row==NIL, row:=row(), NIL)
   iif(col==NIL, col:=col(), NIL)
   obj:=checkbox(row,col,caption)
   obj:buffer:=var
   obj:fblock:=fblock
   obj:sblock:=sblock
   obj:bitmaps:=bitmaps
   obj:message:=message
   if style!=NIL
      obj:style:=style
   endif
   obj:colorSpec:=iif(empty(color),setcolor(),color)
   obj:__setcolor()
   obj:display()
return obj
