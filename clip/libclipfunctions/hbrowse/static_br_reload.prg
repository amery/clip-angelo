static function br_reload()
	::doc:close()
	::doc:init()
	::startHtml := .f.
	(::lists):asize(0)
	::url:open()
	::url:goTop()

	::getUrl()
	::doc:marginLeft := MARGIN_LEFT
	::parseNewUrl()
	::doc:drawBox(.f.)
	::doc:refresh()
return
