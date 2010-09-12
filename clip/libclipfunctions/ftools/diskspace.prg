function diskspace(xDisk)
	if valtype(xDisk)=="N"
		return  diskfree(chr(asc("A")+xDisk-1))
	endif
return diskfree(xDisk)
