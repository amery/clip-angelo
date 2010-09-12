function __GET__(xData,cName,cPic,bValid,bWhen)
	local block
	block:={|_1|local(_2:=@xData), iif(_1==NIL,_2,_2:=_1)}
return getnew(row(),col(),block,cName,cPic,,,bValid,bWhen)
