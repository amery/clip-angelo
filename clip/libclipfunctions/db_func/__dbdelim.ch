function __dbDelim(lIO,file,fdelim,rfields, ufor, uwhile, unext, rec, rest)
	if lIO
		return __dbCopyDelim(file,fdelim,rfields, ufor, uwhile, unext, rec, rest)
		endif
return __dbAppDelim(file,fdelim,rfields, ufor, uwhile, unext, rec, rest)
