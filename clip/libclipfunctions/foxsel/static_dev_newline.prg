static function dev_newLine()
	::__curLine++
	if ::console .and. ::wait ;
		.and. ( ::__curLine % (maxrow()-1)==0 ) ;
                .and. ::__curLine>0
        	wait [Press any key to continue]
        endif
        if ::__curLine==0
        	if !::plain //.and. !::into
			?
        	endif
        else
        	? ::outBuffer
                ::outBuffer:=""
        endif
return
