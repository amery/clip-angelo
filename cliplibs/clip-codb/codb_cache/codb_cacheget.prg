/***********************************/
function codb_CacheGet(obj_id)
	local k:=cton(right(obj_id,1),32)+1
	__check_data()
	if obj_id $ data[k]
		return data[k][obj_id]
	endif
return NIL
