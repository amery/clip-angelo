/***********************************/
function codb_CacheAdd(obj_id,obj)
	if obj_id $ data
		return
	endif
	data[obj_id] := obj
	if len(data) > CODB_PUBLIC_CACHE*16
		codb_cache_minimize(data, CODB_PUBLIC_CACHE/2 )
	endif
return

