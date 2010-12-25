static function sql_run(self)
	local aname,i,ii,s,data

 	private __self:=self // for use self in subqueries

        if self:subQuery .and. !self:externalData .and. !empty(self:outBuffer)
        	return
        endif

        for ii=1 to len(self:from)
        	aname:=self:from[ii]
                if !empty(self:filters[ii])
        		rddSetFilter(aname:hDb,self:filters[ii])
                endif
        	rddGoTop(aname:hDb)
                if self:functions  // create private variables for fields
                        data:=rddRead(aname:hDB)
                	private &(aname:file):=data
                	s:=aname:dbStruct
                        for i in s
                        	private &(i[1]):=data[ i[1] ]
                        next
                endif
        next
        for s in self:fields
		data:=eval(s:block)
               	s:len:=max(s:len, max(len(data), iif(self:to == SQL_TO_DBF,1,len(s:xname) ) ) )
               	s:dec:=max(dec(data),s:dec)
        next

	out_recursive(self,1)

	if !empty(self:order)
		asort(self:buffer,,,{|x1,x2|__sql_order_by(self:order,x1,x2)})
        endif

return
