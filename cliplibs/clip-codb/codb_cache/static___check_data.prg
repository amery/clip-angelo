/***********************************/
static function	__check_data()
	local i,m
	if data==NIL
		data := {}
		for i=1 to 32
			m := map()
			aadd(data,m)
			m:=NIL
		next
	endif
return

