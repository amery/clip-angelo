static function sql_outResult(self)
	local i,iii,ii,s,data
        /*
        	don`t remake result of query and return old data
                for subQueries as EXIST,ANY,ALL
        */
        if self:subQuery .and. !self:externalData .and. !empty(self:outBuffer)
        	return self:outBuffer
        endif

	self:initOut()
        self:__havingProcess:=.t.

        if self:heading .and. !self:plain //.and. !self:into
		self:newLine()  // out header
        endif

        for s in self:fields
        	s:enable:=.f.
                if self:heading .and. !self:plain //.and. !self:into
                	self:out(s,s:xname)
                endif
        next

	iii:=1
        self:__bufferLine:=0
        for i in self:buffer

        	self:__bufferLine++
                if !eval(self:hFilter,self,self:__bufferLine)
                	loop
                endif
        	self:newLine()
        	for ii in self:fields
                	self:out(ii,i[ii:xname])
                next
        next

	if !empty(self:outBuffer)
		self:newLine()
        endif

	self:closeOut()

        if self:subQuery
        	return self:outBuffer
        endif
return
