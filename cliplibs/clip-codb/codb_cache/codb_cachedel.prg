/***********************************/
function codb_CacheDel(obj_id)
	local k:=cton(right(obj_id,1),32)+1
	__check_data()
	if obj_id $ data[k]
		adel(data[k],obj_id)
	endif
return
