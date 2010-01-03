function recursiveDirectory(cMask, cPath)
local mDir, mD, i, j
	if empty(cPath)
		cPath := ""
	endif
	mDir := directory(cMask, "D")
	for i:= 1 to len(mDir)
		if mDir[i][5] == "D"
			if mDir[i][1] == "." .or. mDir[i][1] == ".."
				adel(mDir, i)
				asize(mDir, len(mDir)-1)
				i--
			else
				if dirChange(mDir[i][1])<0
					adel(mDir, i)
					asize(mDir, len(mDir)-1)
					i--
					loop
				endif
				mD := recursiveDirectory(cMask, cPath+mDir[i][1]+"/")
				dirChange("../")
				adel(mDir, i)
				asize(mDir, len(mDir)+len(mD)-1)
				for j:=1 to len(mD)
					ains(mdir, i, mD[j])
				next
				i+=len(mD)-1
			endif
		else
			mDir[i][1] := cPath+mDir[i][1]
		endif
	next
return mDir
