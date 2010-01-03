static function progerr(e,n)
local k
k:=myalert('BL runtime error in line '+alltrim(str(n)),{'Stop','More...'},'BL')
if k==2
  myalert(ErrorMessage(e),{'Ok'})
endif
break(e)
return e
