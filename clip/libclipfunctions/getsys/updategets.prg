FUNC UpDateGets( list )
	local get, ppp
	iif( pcount()==0, list:=memvar->getlist, NIL)
	aeval(list,{|X|x:display()})
RETURN NIL
