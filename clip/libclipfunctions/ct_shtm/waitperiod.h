function waitperiod(nSec)
	if nSec != NIL
#ifdef USE_TASKS
		lWaitperiod := .t.
		start(@_waitperiod(),nSec)
#endif
	endif
return lWaitperiod
