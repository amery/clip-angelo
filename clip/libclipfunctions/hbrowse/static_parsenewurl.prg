static function parseNewUrl()
local s, arr, i, str:=""
	do while !::parser:empty()
		s := ::parser:get()
		if valtype(s) == "O"
			if !empty(str)
				if !empty(::str) .and. ::newStr
					::addObject(::str)
					::newLine()
					::str := ""
					::newStr := .f.
				endif
				if !empty(::str)
					str := ::str + str
					::str := ""
				endif
				arr := split_string(str, ::width, ::width-(::doc:n_Pos-::doc:marginLeft))

				if ::center
					::doc:n_Pos := (::width-len(arr[1]))/2+::doc:marginLeft
				endif
				//str := iif(::center, padc(arr[1], ::width), arr[1])
				str := arr[1]
				::addObject(str)
				for i=2 to len(arr)
					::newLine()
					if ::center
						::doc:n_Pos := (::width-len(arr[i]))/2+::doc:marginLeft
					endif
					str := arr[i]
					::addObject(str)
				next
				arr:={}
				str := ""
			endif
			if !empty(::str) .and. ::newStr
				::addObject(::str)
				::newLine()
				::str := ""
				::newStr := .f.
			endif
			if !empty(::str)
				str := ::str + str
				::str := ""
			endif
			::parseTag(s)
			loop
		endif
		if !::startHtml
			loop
		endif
		if empty(s) //.and. !isRefer
			loop
		endif
		s=tabexpand(s, TAB_SIZE)
		if ::pre
			arr := split(s, "[&\n]")
			for i=1 to len(arr)
				::addObject(replace_symbols(arr[i], .f.))
				::newLine()
			next
			loop
		endif
		//str += iif(!empty(str), " ", "")+ltrim(s)
		//str += iif(!empty(str), " ", "")+s
		str += s
	enddo
return
