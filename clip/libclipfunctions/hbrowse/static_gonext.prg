static function goNext()
	::indStory ++
	if ::indStory>len(::Story)
		alert("It is the last document!")
		::indStory --
	else
		::newRefer(::Story[::indStory]:url, .f.)
	endif
return
