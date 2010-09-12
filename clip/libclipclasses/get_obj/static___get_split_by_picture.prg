static function __get_split_by_picture(arr,p,str,s1,s2)
	local p1,p2
	p1:=arr[p]
	for p2=p to len(arr)
	      if arr[p2] != (p1)
		  exit
	      endif
	      p1++
	next
	p1--
	s1:=substr(str,1,p1)
	s2:=substr(str,p1+1)
return
