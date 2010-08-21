function copy_scr(x1,x2,x3,x4,x5,x6,x7,x8)
	local scr
	scr=SAVESCREEN(x1,x2,x3,x4)
	RESTSCREEN(x5,x6,x7,x8,scr)
RETURN
