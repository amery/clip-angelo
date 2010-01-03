STATIC FUNCTION connTestParser(sql,pars)
	ParLocWrite(::conn,pars,::RDBMS)
RETURN SQLTestParser(::conn,sql,pars)
