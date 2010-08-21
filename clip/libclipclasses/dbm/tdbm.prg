function tDbm()
	local obj	:= map()
        obj:classname	:= "TDBM"
        obj:afields	:= {}
        obj:aindexes	:= {}

        obj:new		:= @tdb_new()
        obj:create	:= @tdb_create()
        obj:redefine	:= @tdb_redefine()
        obj:addField	:= @tdb_addField()
        obj:addIndex	:= @tdb_addIndex()
        obj:readFields	:= @tdb_readFields()
 //       obj:readIndexes	:= @tdb_readIndexes()
        obj:writeFields	:= @tdb_writeFields()
//        obj:writeIndexes:= @tdb_writeIndexes()

return 	obj
