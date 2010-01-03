function __dbSDF(lIO,file,rfields, ufor, uwhile, unext, rec, rest)
	if lIO
		return __dbCopySDF(file,rfields, ufor, uwhile, unext, rec, rest)
	endif
return __dbAppSDF(file,rfields, ufor, uwhile, unext, rec, rest)
