static function __SA_delItem(pos)
	::error:=""
	if pos<0 .or. pos > len(::items)
		::error:=[Out bounds in sorted array]
		return .f.
	endif
	adel(::items,pos)
	asize(::items,len(::items)-1)
return .t.
