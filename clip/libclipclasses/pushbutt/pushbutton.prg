function pushButton(nrow,ncol,caption)
   local obj
   obj:=map()
   obj:classname:="PUSHBUTTON"
   obj:bitmap	:=NIL
   obj:bmpXOff	:=NIL
   obj:bmpYOff	:=NIL
   obj:buffer	:=.f.
   obj:caption	:=caption
   obj:capRow	:=nrow
   obj:capcol	:=ncol
   obj:capXOff	:=NIL
   obj:capYOff	:=NIL
   obj:cargo	:=NIL
   obj:col	:=ncol
   obj:colorSpec:=setcolor()
   obj:fblock	:=NIL
   obj:hasFocus	:=.f.
   obj:message	:=""
   obj:row	:=nrow
   obj:sblock	:=NIL
   obj:sizeX	:=NIL
   obj:sizeY	:=NIL
   obj:style	:="<>"
   obj:typeOut	:=.f.

   //obj:capCol	:=ncol+4
   //obj:capRow	:=nrow


   obj:__cursor	:=setcursor()
   obj:__colors	:={}

   obj:winbuffer := nil

   _recover_pushbutton(obj)

   obj:__setcolor()

return obj
