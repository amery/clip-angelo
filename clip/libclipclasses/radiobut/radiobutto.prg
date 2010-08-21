function radioButto(nrow,ncol,caption,rvalue)
   local obj
   obj:=map()
   obj:classname:="RADIOBUTTON"
   obj:bitmaps	:=NIL
   obj:buffer	:=.f.
   obj:caption	:=caption
   obj:capCol	:=ncol+4
   obj:capRow	:=nrow
   obj:cargo	:=NIL
   obj:col	:=ncol
   obj:colorSpec:=setcolor()
   obj:fblock	:=NIL
   obj:hasFocus	:=.f.
   obj:message	:=""
   obj:row	:=nrow
   obj:rvalue	:=rvalue
   obj:sblock	:=NIL
   obj:style	:="(* )"
   obj:typeOut	:=.f.

   obj:__colors:={}

   _recover_radiobutton(obj)

   obj:__setcolor()

return obj
