static function WriteToFile(fileName)
	local ret,file,cons_old:=set(_SET_CONSOLE)

        file:=iif(fileName==NIL,::fileName,fileName)
        if file!=NIL .and. !empty(file)
        	set(_SET_CONSOLE,.f.)
        	set printer on
        	set printer to (file)
        endif
	ret=::__write()
        set printer off
        set printer to
        set(_SET_CONSOLE,cons_old)
return ret
