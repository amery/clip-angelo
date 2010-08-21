function inkey(time,mask)
	local ret:=0, skey:=0,stat := 0
	if nextkey() != 0
		ret := _inkey(time,mask)
		return ret
	endif
	if set(_SET_IDLE_INKEY)
		return _inkey(time,mask)
	endif
	if !empty(t_buffer)
		ret:=asc(left(t_buffer,1))
		t_buffer := substr(t_buffer,2)
		return ret
	endif
	if time == NIL  .or. time != 0 .or. t_recursive != 0
		ret := _inkey(time,mask)
		return ret
	endif
	if ta_proc == NIL .and. tp_proc == NIL // .or. tp_mode == NIL
#ifdef ___1
		ret := _inkey(time,mask)
#else
		/* keyboard(data) from paralell task */
		while ret==0
			ret := _inkey(0.1,mask)
		end
#endif
		return ret
	endif
	while .t. //nextkey() == 0
		sKey := scankey(time)
		switch skey
			case 29
			case 42
			case 54
			case 56
			case 58
			case 69
			case 70
			otherwise
				ret:=_inkey(0.5)
				if ret != 0
					if ta_proc != NIL .and. t_recursive==0
						t_recursive ++
						clip(ta_proc,ret)
						t_recursive --
					else
						exit
					endif
				endif
				if nextkey() != 0 .or. !empty(t_buffer)
					exit
				endif
		end
		stat := kbdstat()
		if tp_proc != NIL .and. numand(stat,tp_mode) != 0 .and. t_recursive==0
			t_recursive ++
			clip(tp_proc,stat)
			t_recursive --
			loop
		endif
	end
return ret
