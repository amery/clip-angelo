function __sql_getValue(hDB,fname,xname,self)
	if self!=NIL .and. self:__havingProcess
        	return self:buffer[self:__bufferLine][xname]
        endif
return rddGetValue(hDb,fname)
