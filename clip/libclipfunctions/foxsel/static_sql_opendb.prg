static function sql_openDB()
	local i,j
        for i in ::from
        	if i:hDB==NIL
                	if (j:=getalias(i:file)) > 0
				i:hDB:=rddHandle(j)
				i:Open:=.f.
                        else
        			i:hDB:=rddUseArea(,i:file,.t.,.f.)
				i:Open:=.t.
                        	if file(i:file+memoext())
        				rddSetMemo(i:hDB,,i:file)
                        	endif
                        	if file(i:file+indexext())
        				rddSetIndex(i:hDB,,i:file)
                        	endif
                        endif
                        i:dbstruct:=rddStruct(i:hDB)
                endif
        next
return
