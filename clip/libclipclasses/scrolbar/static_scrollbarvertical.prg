static function scrollBarVertical(nstart,nend,noffset,bsBlock)
  local obj
  obj=map()
  obj:subtype   := "VERTICAL"
  obj:style	:= SB_UPSYM+SB_FILLSYM+SB_THUMBSYM+SB_DOWNSYM
  _recover_scrollbar(obj)
return obj
