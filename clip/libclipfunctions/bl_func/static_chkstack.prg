static function chkstack(a)
local i,s
if len(a)==0
 return .t.
endif
for i:=1 to len(a)
  myalert("Uncomplete operator in line "+alltrim(str(a[i][2])),{'Ok'})
next
return .f.
