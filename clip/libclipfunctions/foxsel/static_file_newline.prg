static function file_newLine()
	::__curLine++
        if ::__curLine==0
        	if !::plain //.and. !::into
       			fwrite(::hfile,CRLF)
        	endif
        else
       		fwrite(::hfile,CRLF+::outBuffer)
                ::outBuffer:=""
        endif
return
