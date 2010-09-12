static func PageUp
    ::buffer -= ::itemWin
    ::first -= ::itemWin
    ::clearFindBuffer()
    ::refresh()
RETURN
