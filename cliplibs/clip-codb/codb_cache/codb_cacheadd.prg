/***********************************/
function codb_CacheAdd(obj_id,obj)
	local k:=cton(right(obj_id,1),32)+1
	__check_data()
	if obj_id $ data[k]
		return
	endif
	data[k][obj_id] := obj
	if len(data[k]) > CODB_PUBLIC_CACHE
		codb_cache_minimize(data[k], CODB_PUBLIC_CACHE/4 )
	endif
return
