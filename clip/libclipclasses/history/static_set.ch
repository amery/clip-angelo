static function set(arr, size)
local i, len
	asize(::history, 0)
	len := len(arr)
	for i:=1 to len
		aadd(::history, arr[i])
	next
	if size == NIL
		::size := len(::history)
	else
		::size := size
		asize(::history, ::size)
	endif
return
