static function rddobj_setscope(ScopeTop, ScopeBot)
  __CHECKING_HANDLE
	if ScopeBot==nil; ScopeBot := ScopeTop; endif
	rddordscope(::handle,0,ScopeTop)
	rddordscope(::handle,1,ScopeBot)
return
