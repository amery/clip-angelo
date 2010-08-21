function trapshift(proc_name,mode)
	local ret := tp_proc
	tp_proc := proc_name
	if valtype(mode) == "N"
		tp_mode := mode
	else
		tp_mode := 0
	endif
return ret
