static function out_recursive(self,level)
	local data,idata,stru,i,j,maxLevel:=len(self:from)
        local hdb:=self:from[level]:hDB

        rddGotop(hdb)

       	while !rddEof(hdb)
                if self:functions
                       	data:=rddread(hdb)
                       	memvarSet(self:from[level]:file,data)
                        for i in self:from[level]:dbStruct
                        	memvarSet(i[1],data[ i[1] ])
                        next
                endif
                if level==maxLevel .and. eval(self:filter) //.and. eval(self:hFilter)
       			self:append()
       			for i in self:fields
                        	idata:=eval(i:block,i,self:__bufferLine)
	                	i:len:=max(len(idata),i:len)
	                	i:dec:=max(dec(idata),i:dec)
               			self:toBuffer(i,idata)
       			next
                endif
                if level<maxLevel
			out_recursive(self,Level+1)
                endif
               	rddSkip(hdb)
       	enddo
return
