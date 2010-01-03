static func showview(fdp)
local i
if fdp:listobj:itemCount > 0
	dispbegin()
	i := fdp:listobj:buffer
	@ fdp:nBottom, fdp:nLeft say padr(substr(fdp:viewitem[i], 1, at("|", fdp:viewitem[i])-1), fdp:length-4)
	@ fdp:nBottom+1, fdp:nLeft say padr(substr(fdp:viewitem[i], at("|", fdp:viewitem[i])+1), fdp:length)
	devpos(fdp:listobj:line, fdp:listobj:col)
	dispend()
endif
return
