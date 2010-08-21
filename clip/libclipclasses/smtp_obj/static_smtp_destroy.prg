static function smtp_destroy()
	if ::handle != NIL
		tcpwrite(::handle,"QUIT"+::EOL,,::timeout)
		tcpclose(::handle)
		::handle := NIL
	endif
	if ferror()!=0
		::error := ferrorstr()
	endif
return ferror() == 0
