static function fx_sql_error(message,line)
	local i:=errorNew()
        outlog(3,__FILE__,__LINE__,procname(1),procline(1))
        outlog(3,__FILE__,__LINE__,procname(2),procline(2))
        outlog(3,__FILE__,__LINE__,procname(3),procline(3))
	i:description:="Sql syntax error "
        i:genCode:=EG_SYNTAX
        i:operation:=message
        i:filename:=__FILE__
        i:subSystem:="SQL"
        i:subCode:=line
        //break(i)
        eval(errorblock(),i)
return
