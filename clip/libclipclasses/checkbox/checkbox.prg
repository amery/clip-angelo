function checkbox(nrow,ncol,caption)
   local obj
   obj:=map()
   obj:classname:="CHECKBOX"
   obj:bitmaps	:=NIL
   obj:col	:=ncol
   obj:row	:=nrow
   obj:buffer	:=.f.
   obj:capCol	:=ncol+4
   obj:capRow	:=nrow
   obj:caption	:=caption
   obj:cargo	:=NIL
   obj:colorSpec:=setcolor()
   obj:fblock	:=NIL
   obj:hasFocus	:=.f.
   obj:message	:=""
   obj:sblock	:=NIL
   obj:style	:="[X ]"
   obj:typeOut	:=.f.
   obj:__colors	:={}

   _recover_checkbox(obj)

   obj:__cursor :=setcursor()
   obj:__setcolor()

return obj
