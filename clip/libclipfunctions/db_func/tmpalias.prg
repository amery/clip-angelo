func tmpalias
local i, alias:="_tmp"
i:=1
while select(alias)!=0
  alias+=alltrim(str(i))
  i++
enddo
return alias
