static function sql_closeOut()
	if ::hFile!=NIL
        	fclose(::hFile)
                ::hFile:=NIL
        endif
        if ::__sets!=NIL
        	restoreSets(::__sets)
                ::__sets:=NIL
        endif
return
