static function sql_closeDB()
	local i
        if valtype(::from)=="C"
        	return
        endif
        for i in ::from
        	if i:hDB!=NIL.and.i:Open
        		rddCloseArea(i:hDB)
                        i:hDB:=NIL
                endif
        next
return
