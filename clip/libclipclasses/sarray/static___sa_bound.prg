static function __SA_bound(pos)
	::error:=""
	if pos<1 .or. pos > len(::items)
		::error:=[Out from bound in sorted array]
		return .f.
	endif
return .t.
