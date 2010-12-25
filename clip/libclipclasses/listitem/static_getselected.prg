static function getSelected()
local ret:={}
	/*
	for i=1 to ::itemCount
		if ::item[i]:select
			aadd(ret, ::item[i]:item)
		endif
	next
	*/
	aeval(::item, {|item| iif(item:select, aadd(ret, item:item), NIL)})
	if len(ret) == 0
		aadd(ret, ::getItem())
	endif
return ret
