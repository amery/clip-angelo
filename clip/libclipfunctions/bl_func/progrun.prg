function progrun(aCode)
local errblk
static e
private blStack:={},gpc
e:=errorblock({|err| progerr(err,m->gpc)})
begin sequence
realrun(aCode)
recover using errblk
end sequence
errorblock(e)
release blStack
return nil
