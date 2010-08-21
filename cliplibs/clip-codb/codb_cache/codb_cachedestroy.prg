/***********************************/
function codb_CacheDestroy()
	local i
	for i=1 to len(data)
		afill(data[i],NIL)
	next
	data := NIL
return

