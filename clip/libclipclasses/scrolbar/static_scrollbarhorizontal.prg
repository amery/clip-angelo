static function scrollBarHorizontal(nstart,nend,noffset,bsBlock)
  local obj
  obj=map()
  obj:subtype   := "GORIZONTAL"
  obj:style	:= SB_LEFTSYM+SB_FILLSYM+SB_THUMBSYM+SB_RIGHTSYM
  _recover_scrollbar(obj)
return obj
