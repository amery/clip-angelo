static function arr_newLine()
	local s
	::__curLine++
        if ::__curLine<=0
        	return
        endif
        s:="m->"+::filename
        aadd(&s,::outBuffer)
        ::outBuffer:={}
return
