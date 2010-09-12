* initiation list items
static function initItem(fdp)
local s, c, a, b, i, error

	BEGIN SEQUENCE
	ERRORBLOCK({|e| alert(e:description+';'+e:operation), break(e)})
	c := curdir()
	fdp:current := strtran(currdrive()+"/"+iif(empty(c), "", c+"/"), "//", "/")
	fdp:viewitem := {}
	fdp:listobj:clear()

	//a := fileseek(current, 16)
	a := fileseek(fdp:current, FA_DIRECTORY)
	b := tSortedArrayNew()
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
	b:add(a+"/", a+"|"+padr(padr(fileattrs(), 5)+"| "+str(filesize()), (fdp:nRight-fdp:nLeft)-23 )+"| "+filetime()+" | "+dtoc(filedate()))
	a := fileseek()
    enddo
    for i=1 to b:len()
	fdp:listobj:addItem(b:getKey(i))
	aadd(fdp:viewitem, b:getData(i))
    next

    b = tSortedArrayNew()
    a = fileseek(fdp:current+fdp:mask, FA_ARCHIVE+FA_VOLUME+FA_SYSTEM+FA_HIDDEN+FA_READONLY)
    //a = fileseek(current+mask, 32)
    while !empty(a)
	b:add(a, a+"|"+padr(padr(fileattrs(), 5)+"| "+str(filesize()), (fdp:nRight-fdp:nLeft)-23 )+"| "+filetime()+" | "+dtoc(filedate()))
	a = fileseek()
    enddo
    for i=1 to b:len()
	fdp:listobj:addItem(b:getKey(i))
	aadd(fdp:viewitem, b:getData(i))
    next

    fdp:getobj:setFocus()
    fdp:getobj:killFocus()
    fdp:getobj:varPut(fdp:current+fdp:mask+DOP)
    fdp:getobj:setFocus()
    fdp:listobj:refresh()
    END SEQUENCE
return
