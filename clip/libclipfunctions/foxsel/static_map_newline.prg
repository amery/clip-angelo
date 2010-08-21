static function map_newLine()
	local s,i
	::__curLine++
        if ::__curLine<=0
        	return
        endif
        s:="m->"+::filename
        s:=&s
        for i in s  KEYS
		aadd(s[i],::outBuffer[i])
        next
        ::outBuffer:=map()
return
