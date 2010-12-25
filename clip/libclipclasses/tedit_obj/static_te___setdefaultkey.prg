static function te___setDefaultKey()
local m
	::__keys := map()
	m := ::__keys

	m[K_DOWN]       := {|oTe, nkey| oTe:down(), TE_CONTINUE }
	m[K_PGDN]       := {|oTe, nkey| oTe:pageDown(), TE_CONTINUE }
	m[K_CTRL_PGDN]  := {|oTe, nkey| oTe:bottom(), TE_CONTINUE }
	m[K_UP]         := {|oTe, nkey| oTe:up(), TE_CONTINUE }
	m[K_PGUP]       := {|oTe, nkey| oTe:pageUp(), TE_CONTINUE }
	m[K_CTRL_PGUP]  := {|oTe, nkey| oTe:top(), TE_CONTINUE }
	m[K_HOME]       := {|oTe, nkey| oTe:home(), TE_CONTINUE }
	m[K_END]        := {|oTe, nkey| oTe:end(), TE_CONTINUE }
	m[K_LEFT]       := {|oTe, nkey| oTe:left(), TE_CONTINUE }
	m[K_RIGHT]      := {|oTe, nkey| oTe:right(), TE_CONTINUE }
	m[K_CTRL_LEFT]  := {|oTe, nkey| oTe:wordLeft(), TE_CONTINUE }
	m[K_CTRL_RIGHT] := {|oTe, nkey| oTe:wordRight(), TE_CONTINUE }

	m[K_BS]         := {|oTe, nkey| oTe:backSpace(), TE_CONTINUE }
	m[K_CTRL_BS]    := {|oTe, nkey| oTe:delHome(), TE_CONTINUE }
	m[K_DEL]        := {|oTe, nkey| oTe:delRight(), TE_CONTINUE }
return
