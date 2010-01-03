static function li_select(it)
local i
	it := iif(it==NIL, ::buffer, it)
	if valtype(it)=="A"
		for i=1 to len(it)
			if valtype(it[i])== "N" .and. between(it[i], 1, ::itemCount)
				::item[it[i]]:select := !::item[it[i]]:select
				if ::item[it[i]]:select
					::nSelected += 1
				else
					::nSelected -= 1
				endif
			endif
		next
	elseif valtype(it)=="N".and. between(it, 1, ::itemCount)
		::item[it]:select := !::item[it]:select
		if ::item[it]:select
			::nSelected += 1
		else
			::nSelected -= 1
		endif
	endif
	::refresh()
return .t.
