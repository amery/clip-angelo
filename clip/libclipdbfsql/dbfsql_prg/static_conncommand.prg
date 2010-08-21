STATIC FUNCTION connCommand(sql,pars)
	LOCAL stmt
	LOCAL ret

	SQLLocWrite(::conn,@sql)
	stmt := SQLPrepare(::conn,sql)
	ParLocWrite(::conn,pars,::RDBMS)
	ret := SQLCommand(stmt,pars)
	SQLFreeSTMT(stmt)
RETURN ret
