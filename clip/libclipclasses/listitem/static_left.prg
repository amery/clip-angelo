static func left()
    ::pos --
    ::buffer -= ::rowWin
    ::clearFindBuffer()
    ::refresh()
return
