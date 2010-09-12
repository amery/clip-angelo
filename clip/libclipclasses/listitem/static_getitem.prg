static func getItem()
    if empty(::item)
	return NIL
    endif
return ::item[::buffer]:item
