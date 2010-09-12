static function reload()
  local v
#ifdef DEBUG
	outlog(__FILE__,__LINE__,"__reload",::name)
#endif
   if ::block==NIL
	return NIL
   endif

  ::original:=eval(::block)
  ::__updateInfo()

  ::__analizePic()
  ::__winLen=min(::__winlen,len(::__format))

  if ::type=="N"
       ::__original:=str(::_original,::__len,::__dec)
       ::__original:=strtran(::__original,".","")
  endif

  ::__fillBuffer()
  if len(::__posArr)>0
     ::pos:=::__posArr[1]
  endif

return NIL
