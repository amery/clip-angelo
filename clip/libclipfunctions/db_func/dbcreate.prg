function dbCreate(fname, aStru, rdd, new, al)
	_dbcreate(fname,aStru,rdd)
	if new!=NIL .and. new
		use (fname) via (rdd) alias (aL) NEW
	endif
return
