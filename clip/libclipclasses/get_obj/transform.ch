function transform(xValue,picture)
  local obj	:=map()
  obj:original	:=xValue
  obj:_original	:=xValue
  obj:picture	:=picture
  obj:block	:=NIL
  obj:col	:=0
  obj:expand	:=.f.
  obj:__fullScreen := .f.

  __updateInfo(obj)
  __analizePic(obj)

  if obj:type=="N"
       obj:__original:=str(obj:_original,obj:__len,obj:__dec)
       obj:__original:=strtran(obj:__original,".","")
  endif

  //__fillTransform(obj)
  __fillBuffer(obj)

return __toString(obj)
