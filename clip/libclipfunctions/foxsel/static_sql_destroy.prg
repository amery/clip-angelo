static function sql_destroy()
	local i
	::closeOut()
        ::closeDB()
        for i in ::constants
        	if "CLASSNAME" $ i .and. i:classname=="SQLSELECT"
                	i:closeOut()
                	i:closeDB()
                endif
        next
return
