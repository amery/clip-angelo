static func find(ch)
local i, j, cur, oldfirst, oldline, oldbuff, oldpos, oldfb, found:=.f., cw

    oldfirst := ::first
    oldline := ::line
    oldbuff := ::buffer
    oldpos  := ::pos
    oldfb   := ::findbuffer
    if empty(::findbuffer)
	cur := 1
	cw := 1
	::first := 1
	::line  := ::nTop
	::buffer:= 1
	::pos   := 1
    else
	cur := ::buffer
	cw := ::pos*::rowWin-(::nBottom-::line)
    endif
    ::findbuffer += ch
    for j=cw to ::itemWin
	if cur<=::itemCount .and. "ITEM"$::item[cur] .and. !empty(::item[cur]:item) ;
		.and. ::item[cur]:item = ::findbuffer
		::buffer := cur
		found := .t.
		::refresh()
		return found
	endif
	cur++
    next

    ::line := ::nBottom
    ::pos := ::colWin
    ::first ++

    if cur<=::itemCount .and. "ITEM"$::item[cur] .and. !empty(::item[cur]:item)
       for i=cur to ::itemCount
	   if ::item[i]:item = ::findbuffer
	      ::buffer := i
	      found := .t.
	      ::refresh()
	      return found
	   endif
	   ::first ++
	next
    endif
    if !found
	::first := oldfirst
	::line  := oldline
	::buffer:= oldbuff
	::pos   := oldpos
	::findbuffer := oldfb
    endif
    ::refresh()
RETURN found
