static func delItem(position)
    if !between(position, 1, ::itemCount)
	return 0
    endif
    adel(::item, position)
    //adel(::colorItem, position)
    ::itemCount --
    asize(::item, ::itemCount)
    //asize(::colorItem, ::itemCount)
    if ::buffer > ::itemCount
	::buffer := ::itemCount
    endif
    ::refresh()
return 1
