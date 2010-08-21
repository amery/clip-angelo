static function te_loadString(str)
local ts
	if str==NIL .or. (valtype(str)!="C" .and. valtype(str)!="M")
		return .f.
	endif

	::__nullInit()
	if ::charset != NIL
		str := translate_charset(::charset, ::__hostcharset, @str)
	endif
	::edbuffer := charsplit("&\n", @str)
	ts := ::tabsize
	aeval(::edbuffer, {|item| item := tabexpand(item, ts)})
	::lines:=len(::edbuffer)
	::refresh()
RETURN  .t.
