static function parse_Meta(tag)
local arr, i
	if "CONTENT"$tag:fields
		arr := split(tag:fields:CONTENT, ";")
		for i in arr
			if lower(alltrim(i)) = "charset="
				::charset := substr(i, at('=', i)+1)
				exit
			endif
		next
	endif
return
