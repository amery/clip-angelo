static function db_makeStr(self, item)
local i, str:="", pr
	pr := 0
	for i:=1 to len(item)
		str += padl(padr(item[i]:txt, item[i]:nr-item[i]:nl+1), item[i]:nr-pr)
		pr := item[i]:nr
	next
return str
