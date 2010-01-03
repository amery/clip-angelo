static function updateBuffer()
  local v
#ifdef DEBUG
	outlog(__FILE__,__LINE__,"updatebuffer",::name)
#endif
   if ::block==NIL
	return NIL
   endif

  ::__updateInfo()

  ::__analizePic()
  ::__winLen=min(::__winlen,len(::__format))

  if ::type=="N"
       ::__original:=str(::_original,::__len,::__dec)
       ::__original:=strtran(::__original,".","")
  endif

  ::__fillBuffer()
  ::_display()

return NIL
