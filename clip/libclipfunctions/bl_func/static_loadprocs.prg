static function loadprocs(s,a)
local lines,i
lines:=lineparse(memoread(s),x_EOL)
for i:=1 to len(lines)
  apush(a,lines[i])
next
return nil
