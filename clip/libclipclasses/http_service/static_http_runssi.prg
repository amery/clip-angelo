static function HTTP_runSSI(self,data)
	local i,j,beg,inc,end:="",s,s1,s2
	local url,file
	s := data
/*
<!--#include virtual="/top.inc"-->
*/

	while (i := at("<!--#include ",s)) > 0
		beg := left(s,i-1)
		s := substr(s,i+13)
		i := at("-->",s)
		if i<=0
			i := len(s)+1
		endif
		end := substr(s,i+3)
		***
		inc := ""
		s1 := left(s,i-1)
		i := at("virtual=",s1)
		if i<=0
			s := beg+" Bad 'include' format. "+end
			loop
		endif
		s1 := substr(s1,i+9)
		url := strtran(s1,'"','')
		file := makePath(url)
		if !file(file)
			s := beg+' Included URL '+url+' not found. '+end
			loop
		endif
		inc := memoread(url)
		***
		s := beg+inc+end
	enddo
return s
