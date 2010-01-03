static function url_makeUrl(full)
	local ret:="",user:=.f., i
	full := iif(full==NIL, .t., full)
	ret+=::protocol+":"
	if ::port!=NIL
		ret+=alltrim(str(::port,4,0))
	endif
	if ::user!=NIL .and. ::password!=NIL
		ret+="//"+::user+":"+::password+"@"
		user:=.t.
	endif
	if !empty(::host)
		if user
			ret+=::host
		else
			ret+="//"+::host
		endif

	endif
	ret+=::path+"/"+::file
	if full .and. ::params!=NIL
		ret+="?"+::params
	endif
	if full .and. ::anchor!=NIL
		i := at("#", ret)
		if i>0
			ret:=substr(ret, 1, i-1)
		endif
		ret+="#"+::anchor
	endif
return ret
