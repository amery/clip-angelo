********************************
static function me___check_share(fd, num)
local ret:=2, i, f:= .f.
	if empty(fd)
		return 0
	endif
	for i:= 1 to ::nWins
		if ::tobjinfo[i]:name == fd
			f := .t.
			num := i
			exit
		endif
	next
	if f
		ret := alert("Window exist:;"+fd, {[Cancel], [New], [Link]})
	endif
return ret
