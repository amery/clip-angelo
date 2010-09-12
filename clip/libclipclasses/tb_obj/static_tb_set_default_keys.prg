static function	tb_set_default_keys()
	local m
	::__keys := map()
	m:=::__keys
	m[K_DOWN] 	:= { |oTb,nKey| oTb:down(), 	TBR_CONTINUE }
	m[K_UP] 	:= { |oTb,nKey| oTb:up(), 	TBR_CONTINUE }
	m[K_END]  	:= { |oTb,nKey| oTb:end(), 	TBR_CONTINUE }
	m[K_HOME]  	:= { |oTb,nKey| oTb:home(), 	TBR_CONTINUE }
	m[K_LEFT]  	:= { |oTb,nKey| oTb:left(), 	TBR_CONTINUE }
	m[K_RIGHT]  	:= { |oTb,nKey| oTb:right(), 	TBR_CONTINUE }
	m[K_PGDN]  	:= { |oTb,nKey| oTb:pageDown(), TBR_CONTINUE }
	m[K_PGUP]  	:= { |oTb,nKey| oTb:pageUp(), 	TBR_CONTINUE }
	m[K_CTRL_PGDN]  := { |oTb,nKey| oTb:goBottom(), TBR_CONTINUE }
	m[K_CTRL_PGUP]  := { |oTb,nKey| oTb:goTop(), 	TBR_CONTINUE }
	m[K_CTRL_END]  	:= { |oTb,nKey| oTb:panEnd(), 	TBR_CONTINUE }
	m[K_CTRL_HOME]  := { |oTb,nKey| oTb:panHome(), 	TBR_CONTINUE }
	m[K_CTRL_LEFT]  := { |oTb,nKey| oTb:panLeft(), 	TBR_CONTINUE }
	m[K_CTRL_RIGHT] := { |oTb,nKey| oTb:panRight(),	TBR_CONTINUE }

	m[K_ESC]  	:= { |oTb,nKey| TBR_EXIT }
//        m[K_ENTER]  	:= { |oTb,nKey| TBR_EXIT }
	m[K_LBUTTONDOWN]:= { |oTb,nKey| tbMouse(oTb,mrow(),mcol()) }
return
