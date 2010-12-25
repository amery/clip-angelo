static function stripSpace(str, pos)
local arr:={}
	while search("  ", str, arr, pos)
		if arr[1][2] - arr[1][1] > 1
			str := substr(str, 1, arr[1][1]-1)+" "+substr(str, arr[1][2])
		endif
		arr := {}
	enddo
return str
