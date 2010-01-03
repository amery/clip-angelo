static func showview(fdp)
local i

i := fdp:listobj:buffer
@ fdp:nBottom, fdp:nLeft say padr(substr(fdp:viewitem[i], 1, at("|", fdp:viewitem[i])-1), fdp:length)
@ fdp:nBottom+1, fdp:nLeft say padr(substr(fdp:viewitem[i], at("|", fdp:viewitem[i])+1), fdp:length)
devpos(fdp:listobj:line, fdp:listobj:row)
return
