function sqlQueryNew()
	local obj:=map()

        obj:classname	:= "SQLSELECT"
        obj:subQuery	:= .f. //it`s called as subquery for EXIST,ALL,ANY ?
        obj:subQueryType:= 0   // EXIST | ANY | ALL | ...
        obj:externalData:= .f. //in query expr exist access for external values
        obj:fields	:= {}  // fields info of result columns
        obj:All		:= .T. // select ALL or DISCINT
        obj:allFields	:= .T. // select ALL or DISCINT for fields list
        obj:from	:= {}  // tables info
        obj:into	:= .F.
        obj:to		:= 0   // file,printer,screen,rdd,array
        obj:filename	:= ""  // for TO operation
        obj:hFile	:= NIL // result fopen(::filename)
        obj:additive	:= .f.  // for TO operation
        obj:console	:= .T.
        obj:plain	:= .F.
        obj:wait	:= .T.
        obj:where	:= ""
        obj:filter	:= {||.t.} // codeblock as filter expessions for result table
        obj:filters	:= NIL     // filter expessions for tables
        obj:hfilter	:= {||.t.} // codeblock as filter HAVING
        obj:constants	:= {}      // constant value for IN and EXIST,ANY,ALL
        obj:group	:= ""
        obj:groupBlocks := {}	   // codeblocks for get value of fields
        obj:groupKeys   := {}	   // groups key data
        obj:having	:= ""
        obj:order	:= NIL     // list of column names for ORDER BY
        obj:functions	:= .f.  // have functions in expressions ?
        obj:gFunctions	:= .f.  // have group functions in expressions ? (SUM,COUNT,...)
        obj:heading	:= .t.  // output header with column names

        obj:__sets	:= NIL  // SET() saved values
        obj:__havingProcess := .f.

 	obj:parse	:= @sql_parse()
 	obj:parseFrom	:= @sql_parseFrom()
 	obj:parseFields	:= @sql_parseFields()
 	obj:parseGroup	:= @sql_parseGroup()
 	obj:parseOrder	:= @sql_parseOrder()
 	obj:parseTo	:= @sql_parseTo()
 	obj:parseExpr	:= @sql_parseExpr()
 	obj:logicalOptimizeExpr:= @sql_logicalOptimizeExpr()
 	obj:makeFilters	:= @sql_makeFilters()

 	obj:openDB	:= @sql_openDB()

 	obj:run		:= @sql_run()

        obj:buffer	:= {}   // buffer for result dataBase or Groups
        obj:__bufferLine:= 0    // index in buffer for group of current record
 	obj:append	:= @sql_append()     // append new result record
 					     // and update group information
 	obj:toBuffer	:= @sql_toBuffer()  // out to bufferred result record

        obj:outBuffer	:= NIL  // buffer for output
        obj:__curLine	:= -1   // output lines
 	obj:initOut	:= @sql_initOut()
 	obj:outResult	:= @sql_outResult()
 	obj:newLine	:= @dev_newLine() //CRLF or aadd() to output
 	obj:out		:= @dev_out()
 	obj:closeOut	:= @sql_closeOut()

 	obj:closeDB	:= @sql_closeDB()
        obj:destroy	:= @sql_destroy()

return obj
