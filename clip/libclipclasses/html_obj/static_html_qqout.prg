static function html_qqout()
	local i,str:=""
        for i=2 to pcount()
        	str+=toString(param(i))+","
        next
        str:=substr(str,1,len(str)-1)
return  ::addData(str)
