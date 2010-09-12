static func __init()
local m
	::__keys := map()
	m := ::__keys
	m[K_LEFT]	:= {|oL, nKey| oL:left(), LI_CONTINUE }
	m[K_RIGHT]	:= {|oL, nKey| oL:right(), LI_CONTINUE }
	m[K_UP]		:= {|oL, nKey| oL:up(), LI_CONTINUE }
	m[K_DOWN]	:= {|oL, nKey| oL:down(), LI_CONTINUE }
	m[K_HOME]	:= {|oL, nKey| oL:home(), LI_CONTINUE }
	m[K_END]	:= {|oL, nKey| oL:end(), LI_CONTINUE }
	m[K_PGUP]	:= {|oL, nKey| oL:pageup(), LI_CONTINUE }
	m[K_PGDN]	:= {|oL, nKey| oL:pagedown(), LI_CONTINUE }
	m[K_ESC]	:= {|oL, nKey| LI_EXIT }

return .t.
