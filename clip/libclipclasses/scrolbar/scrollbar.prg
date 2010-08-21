function scrollBar(nstart,nend,noffset,bsBlock,orient)

  local obj
  orient:=iif(orient==NIL,1,orient)
  orient:=max(1,min(2,orient))
  if orient==1
     obj:=scrollBarVertical(nstart,nend,noffset,bsBlock)
  else
     obj:=scrollBarHorizontal(nstart,nend,noffset,bsBlock)
  endif
  obj:classname	:="SCROLLBAR"
  obj:bitmaps	:= NIL
  obj:cargo	:= NIL
  obj:colorSpec	:= setcolor()
  obj:current	:= 1
  obj:end	:= nend
  obj:offset	:= noffset
  obj:orient	:= orient
  obj:sblock	:= bsBlock
  obj:start	:= nstart
  obj:thumbPos	:= 1
  obj:total	:= 100
  obj:barLength	:= max(nend-nstart-1,0)
return obj
