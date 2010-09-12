function _pushbutt_(caption,message,color,fblock,sblock,style,;
		    sizex,sizey,capxoff,capyoff,bitmap,bmpxoff,;
		    bmpyoff,row,col)
   local obj
   row:=iif(row==NIL,row(),row)
   col:=iif(col==NIL,col(),col)
   obj:=pushButton(row,col,caption)
   obj:buffer	:=.f.
   obj:fblock	:=fblock
   obj:sblock	:=sblock
   obj:message	:=message
   obj:sizex	:=sizex
   obj:sizey	:=sizey
   obj:capxoff	:=capxoff
   obj:capyoff	:=capyoff
   obj:bitmap	:=bitmap
   obj:bmpxoff	:=bmpxoff
   obj:bmpyoff	:=bmpyoff
   if style!=NIL
      obj:style:=style
   endif
   obj:colorSpec:=iif(empty(color),setcolor(),color)
   obj:__setcolor()
   obj:display()
return obj
