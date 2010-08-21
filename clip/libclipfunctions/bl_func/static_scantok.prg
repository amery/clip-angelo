static function scantok(aStack,aTok)
local i
for i:=len(aStack) to 1 step -1
  if ascan(aTok,aStack[i][1])<>0
    return i
  endif
next
return 0
