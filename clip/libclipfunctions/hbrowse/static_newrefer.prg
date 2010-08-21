static function newRefer(href, newR)
local i, st, oldf
	newR := iif(newR==NIL, .t., newR)
	if newR
		::Story[::indStory]:line := ::doc:line
		::Story[::indStory]:pos := ::doc:pos
		::Story[::indStory]:row := ::doc:rowWin
		::Story[::indStory]:col := ::doc:colWin
	endif


	oldf := ::url:file
	if newR
		i := "//":at(href)
		if i >0
			::url:parseUrl(href)
		else
			i := "/":rat(href)
			if i <= 0
				if left(href, 1) == "#"
					i := at("#", ::url:file)
					if i>0
						::url:file := substr(::url:file, 1, i-1)
					endif
					::url:file += href
					::url:parseUrl(::url:file)
				else
					::url:file := href
				endif
			else
				::url:file := substr(href, i+1)
				::url:path := substr(href, 1, i-1)
				::url:path := substr(::url:path, "/":at(::url:path))
			endif
			::url:parseUrl(::url:makeUrl())
		endif
		aadd(::Story, map())
		::indStory++
		::Story[::indStory]:url := ::url:makeUrl()
		::Story[::indStory]:line := 1
		::Story[::indStory]:pos := 1
		::Story[::indStory]:row := 1
		::Story[::indStory]:col := 1
	else
		::url:path := ""
		::url:file := ""
		::url:parseUrl(href)
	endif
	st := ::Story[::indStory]
	if ::url:file != oldf
		::doc:close()
		::startHtml := .f.
		(::lists):asize(0)
		i:=::url:open()
		::url:goTop()

		::getUrl()
		::doc:marginLeft := MARGIN_LEFT
		::parseNewUrl()
		::doc:drawBox(.f.)
	endif
	if ::url:anchor==NIL .or. !::doc:goItem(::url:anchor, .t.)
		::doc:goTo(st:line, st:pos, st:row, st:col)
	endif
return
*************
