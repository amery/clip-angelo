static func End
  ::pos := ::colWin
  ::buffer := ::itemCount
  ::first := ::itemCount - (::colWin*::rowWin) + 1
  ::line := ::nBottom- ::rowWin + 1
  ::clearFindBuffer()
  ::refresh()
return
