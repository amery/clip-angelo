static func __resize(nrow,ncol)
    local i
    for i=1 to ::itemCount
	::width=max(::width,__capLength(::__items[i]:caption))
    next
    ::top:=nrow
    ::left:=ncol
    ::right :=::left+::width+3
    i:=::right-maxcol()
    if i>0
	::right-=i
	::left-=i
    endif
    ::bottom:=min(::top+::itemCount+1,maxrow()-1)

return ::itemCount
