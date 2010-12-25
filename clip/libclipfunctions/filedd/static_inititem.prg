static function initItem(fdp)
    local s, c, a, b, i, cc[2]

    fdp:viewitem := {}
    fdp:listobj:clear()
    c := curdir()
    fdp:current := strtran(currdrive()+"/"+iif(empty(c), "", c+"/"), "//", "/")

    //a := fileseek(current, 16)
    a := fileseek(fdp:current, FA_DIRECTORY)
    b := {}
    while !empty(a)
	#ifdef OS_CYGWIN
	if a == ".."
		a := fileseek()
		loop
	endif
	if a == "."
		a := ".."
	endif
	#else
	if a == "."
		a := fileseek()
		loop
	endif
	#endif
	cc[1] := a+"/"
	cc[2] := a+"|"+padr(padr(fileattrs(), 5)+"| "+str(filesize()), (fdp:nRight-fdp:nLeft)-23 )+"| "+filetime()+" | "+dtoc(filedate())
	aadd(b, aclone(cc))
	a := fileseek()
    enddo
    b := asort(b,,,{|x, y| x[1] < y[1]})
    for i=1 to len(b)
	fdp:listobj:addItem(b[i][1])
	aadd(fdp:viewitem, b[i][2])
    next

    b = {}
    a = fileseek(fdp:current+fdp:mask, FA_ARCHIVE+FA_VOLUME+FA_SYSTEM+FA_HIDDEN+FA_READONLY)
    //a = fileseek(current+mask, 32)
    while !empty(a)
	cc[1] := a
	cc[2] := a+"|"+padr(padr(fileattrs(), 5)+"| "+str(filesize()), (fdp:nRight-fdp:nLeft)-23 )+"| "+filetime()+" | "+dtoc(filedate())
	aadd(b, aclone(cc))
	a = fileseek()
    enddo
    b := asort(b,,,{|x, y| x[1] < y[1]})
    for i=1 to len(b)
	fdp:listobj:addItem(b[i][1])
	aadd(fdp:viewitem, b[i][2])
    next

    fdp:getobj:setFocus()
    fdp:getobj:killFocus()
    fdp:getobj:varPut(fdp:current+fdp:mask+DOP)
    fdp:getobj:setFocus()
    fdp:listobj:refresh()
return
