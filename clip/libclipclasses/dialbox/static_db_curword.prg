static function db_curWord()
local word:="", i, ch
	if ::line > ::lines
		return word
	endif
	word := ::makeStr(::inbuffer[::line])
	if len(word) >= ::pos
		for i:= ::pos to 1 step -1
			ch := substr(word, i, 1)
			if !isalpha(ch) .and. !isdigit(ch) .and. ch!="_"
				word := substr(word, i+1)
				exit
			endif
		next
		for i:=1 to len(word)
			ch := substr(word, i, 1)
			if !isalpha(ch) .and. !isdigit(ch) .and. ch!="_"
				word := substr(word, 1, i)
				exit
			endif
		next
	else
		word := ""
	endif
return word
