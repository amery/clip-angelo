static function rddobj_close()
	local ret
  	ret:=iif(::handle<>NIL,rddclosearea(::handle),NIL)
  	::handle:=NIL
  	::search_result:=.f.
return ret
