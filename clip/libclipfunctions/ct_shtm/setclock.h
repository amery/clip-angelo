function setclock()

	if pcount()==0
		time_close:=.t.
	else
		time_close:=.f.
#ifdef USE_TASKS
		start(@ct_showtime(),0,1,.t.)
#endif
	endif

return
