static function goBack()
	::indStory --
	if ::indStory<1
		alert("It is the first document!")
		::indStory ++
	else
		::newRefer(::Story[::indStory]:url, .f.)
	endif
return
