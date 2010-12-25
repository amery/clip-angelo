function waitRddLock(hRdd,nSeconds,lType)
	local stop_time,locker

	if valtype(nSeconds) != "N"
		nSeconds := 10
	endif
	if valtype(lType) != "L"
		lType := .f.
	endif

	locker=iif(lType,rddFlock(hRdd),rddRlock(hRdd))
	if locker
		return .t.
	endif

	stop_time := seconds()+nSeconds
	do while seconds() < stop_time .and. !locker
		locker=iif(lType,rddFlock(hRdd),rddRlock(hRdd))
		sleep(0.1)
	enddo
return  locker
