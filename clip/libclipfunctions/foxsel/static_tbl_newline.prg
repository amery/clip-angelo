static function tbl_newLine()
	if !empty(::outBuffer)
		append blank
        	dbWrite(::outBuffer)
        	::outBuffer:=map()
        endif
return
