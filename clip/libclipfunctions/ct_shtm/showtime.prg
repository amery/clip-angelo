function showtime(x,y,lSeconds,cColor,n12,lAm)

	if pcount()==0
		time_close:=.t.
	else
		time_close:=.f.
#ifdef USE_TASKS
		start(@ct_showtime(),x,y,lSeconds,cColor,n12,lAm)
#endif
	endif

return
