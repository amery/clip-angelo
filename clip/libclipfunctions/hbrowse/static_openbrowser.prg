static function openBrowser()
local h, buf, s, scr, tag, i
	clear screen
	save screen to scr
	::doc := dialogBox(::nTop, ::nLeft, ::nBottom-1, ::nRight)
	::doc:marginLeft := MARGIN_LEFT
	::width -= ::doc:marginLeft
	if !empty(::urlName)
		::url := urlNew()
		::url:parseUrl(::urlName)
	else
		::url := urlNew("http","last","/itk/clipper","index.html")
	endif
	aadd(::Story, map())
	::indStory++
	::Story[::indStory]:url := ::url:makeUrl()
	::Story[::indStory]:line := 1
	::Story[::indStory]:pos := 1
	::Story[::indStory]:row := 1
	::Story[::indStory]:col := 1

	::url:open()
	::parser:=htmlParserNew()
	::getUrl()
	::parseNewUrl()
	//::doc:handleKey(::doc, __self__)
	::doc:drawBox()
	@ ::nBottom, 0 say padr([<   >-go next item  i-info  <F5>-go line  <F6>-find  <F9>-menu  <ESC>,<F10>-exit], ::nRight) color "0/w"
	@ ::nBottom, 1 say "Tab" color "r/w"
	@ ::nBottom, 20 say "i" color "r/w"
	@ ::nBottom, 29 say "F5" color "r/w"
	@ ::nBottom, 43 say "F6" color "r/w"
	@ ::nBottom, 54 say "F9" color "r/w"
	@ ::nBottom, 65 say "ESC" color "r/w"
	@ ::nBottom, 71 say "F10" color "r/w"
	::runing()
	::doc:close()
	::url:close()
	restore screen from scr
return
