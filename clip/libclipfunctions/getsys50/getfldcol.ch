function getFldCol(num)
  num=iif(num==NIL,status:currentGet,num)
  if num>0 .and. num<=len(__getList)
	return  __getList[num]:col
  endif
return -1
