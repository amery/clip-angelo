static function tdb_new(self, cAlias, cFile, cDriver, lShared, lReadOnly,;
		 aFields, aIndexes, oDict )
	local n
        self:cFile:=cFile
        self:cAlias:=cAlias
        self:cDriver:=cDriver
        self:lShared:=lShared
        self:lReadonly:=lReadonly
	self:odict:=odict
//        self:database:=tdatabase():new(cAlias)
   	if aFields != nil
		for n = 1 to len( aFields )
                	self:AddField( aFields[n] )
   	  	next
   	end
   	if aIndexes != nil
          	for n = 1 to len( aIndexes )
                  	self:AddIndex( aIndexes[n] )
   	  	next
   	end
	oDict:AddDb( Self )
	self:WriteFields()
Return Self
