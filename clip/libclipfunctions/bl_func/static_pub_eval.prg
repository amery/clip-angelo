static function pub_eval(p,pc)
local i,v
for i:=1 to len(p)
  v:=alltrim(p[i])
  public &v
next
return pc+1
