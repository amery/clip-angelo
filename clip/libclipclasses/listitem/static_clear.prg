static func clear()
    asize(::item, 0)
    //asize(::colorItem, 0)
    ::buffer := 1
    ::line := ::nTop
    ::pos := 1
    ::clearFindBuffer()
    ::first := 1
    ::itemCount := 0
return
