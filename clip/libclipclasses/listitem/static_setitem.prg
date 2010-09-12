static func setItem(position)
    if !between(position, 1, ::itemCount)
	return 0
    endif
    ::buffer := position
    if ::buffer > ::first+::itemWin
	::first := ::buffer
    endif
    ::refresh()
return 1
