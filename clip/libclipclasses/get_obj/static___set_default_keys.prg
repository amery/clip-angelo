static function	__set_default_keys()
	local m
	::__keys := map()
	m:=::__keys
	m[K_END]  	:= { |oGet,nKey| oGet:end(),0 }
	m[K_HOME]  	:= { |oGet,nKey| oGet:home(),0 }
	m[K_LEFT]  	:= { |oGet,nKey| oGet:left(),0 }
	m[K_RIGHT]  	:= { |oGet,nKey| oGet:right(),0 }
	//m[K_CTRL_END]  	:= { |oGet,nKey| oGet:End(),0 }
	m[K_CTRL_HOME]  := { |oGet,nKey| oGet:Home(),0 }
	m[K_CTRL_LEFT]  := { |oGet,nKey| oGet:wordLeft(),0 }
	m[K_CTRL_RIGHT] := { |oGet,nKey| oGet:wordRight(),0 }
	m[K_DEL]	:= { |oGet,nKey| oGet:delete(),0 }
	m[K_CTRL_T]	:= { |oGet,nKey| oGet:delWordRight(),0 }
	m[K_CTRL_Y]	:= { |oGet,nKey| oGet:delEnd(),0 }
	m[K_CTRL_BS]	:= { |oGet,nKey| oGet:delWordLeft(),0 }
	m[K_BS]		:= { |oGet,nKey| oGet:BackSpace(),0 }
return
