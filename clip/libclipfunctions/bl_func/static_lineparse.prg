static function lineparse(cline,cdelim)
local a:={},n,s
while !empty(cline)
  n:=at(cdelim,cline)
  aadd(a,nil)
  if n<>0
    a[len(a)]:=substr(cline,1,n-1)
    cline:=substr(cline,n+len(cdelim))
  else
    a[len(a)]:=cline
    cline:=''
  endif
end
return a
