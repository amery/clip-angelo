static function url_close()
	::stop:=seconds()
	if ::__wget==NIL .or. lower(::protocol)=="file"
		return .f.
	endif
	fclose(::__wget)
	::__wget:= NIL
	outlog(3,getenv("USER"),"open url:",::makeUrl(), "read",::length,"bytes in",::stop-::start,"seconds")
return .t.
