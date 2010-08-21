*********************************************************
 /* time format "hh:mm:ss" convert to numeric seconds " */
function ssec(t)
	local h:=0,m:=0,s:=0,x
        h:=val(t)
        x:=at(":",t)
        if x>0
        	m:=val(substr(t,x+1))
        endif
        x:=at(":",t,2)
        if x>0
        	s:=val(substr(t,x+1))
        endif
return s+m*60+h*3600
