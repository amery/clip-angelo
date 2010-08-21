function popup(ntop,nleft,nbottom,nright)
  local obj

  obj		:= map()
  obj:classname	:= "POPUPMENU"
  obj:border	:= NIL //B_SINGLE+" "+SEPARATOR_SINGLE
  obj:bottom	:= nbottom
  obj:cargo	:= NIL
  obj:colorSpec	:= setcolor()
  obj:current	:= 0
  obj:__begItem	:= 1 // for scrolling items up/down
  obj:itemCount	:= 0
  obj:left	:= nleft
  obj:right	:= nright
  obj:top	:= ntop
  obj:width	:= 0
  obj:isopen    := .f.

  _recover_popupmenu(obj)

  obj:__items	:= {}
  obj:__colors	:= {}
  obj:__scr	:= NIL

return obj
