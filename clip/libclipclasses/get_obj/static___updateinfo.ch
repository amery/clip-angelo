static function __updateInfo()
local v,s
#ifdef DEBUG
	outlog(__FILE__,__LINE__,"__updateinfo",::name)
#endif
  if ::block!=NIL
	::_original:=eval(::block)
	s:=::_original
  endif
  ::type:=valtype(::_original)
  //::changed:=.f.
  v:=::_original
  do case
     case ::type=="A"
	  v:=LANG_ARRAY_STRING
     case ::type=="B"
	  v:=LANG_CBLOCK_STRING
     case ::type=="C"
	  v:=v
     case ::type=="T"
	  v:=TTOC(v)
     case ::type=="D"
	  v:=DTOC(v)
     case ::type=="L"
	  v:=iif(v,LANG_YES_STRING,LANG_NO_STRING)
     case ::type=="M"
	  v:=v
     case ::type=="N"
	  v:=str(v)
     case ::type=="O"
	  v:=LANG_OBJECT_STRING
     case ::type=="U"
	  v:="" //LANG_UNDEF_STRING
     otherwise
	  v:="" //LANG_UNKNOWN_STRING
  endcase
  ::__original=v
  ::decpos:=0

return
