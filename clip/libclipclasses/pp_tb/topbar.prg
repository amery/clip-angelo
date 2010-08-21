function topbar(nrow,nleft,nright)
  local obj
  if nrow==NIL .or. nleft==NIL .or. nright==NIL
     return NIL
  endif
  obj		:= map()
  obj:className	:= "TOPBARMENU"
  obj:cargo	:= NIL
  obj:colorSpec	:= setcolor()
  obj:current	:= 0
  obj:itemCount	:= 0
#ifdef 1
  obj:left	:= iif(nleft==NIL,0,nleft)
  obj:right	:= iif(nright==NIL,maxcol(),nright)
  obj:row	:= iif(nrow==NIL,0,nrow)
#else
  obj:left	:= nleft
  obj:right	:= nright
  obj:row	:= nrow
#endif
  obj:width	:= 0
  obj:__begItem := 0

  _recover_topbarmenu(obj)

  obj:__items	:= {}
  obj:__colors	:= {}

return obj
