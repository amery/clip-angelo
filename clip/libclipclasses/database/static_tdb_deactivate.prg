static function tdb_deActivate()
	local ret
	 ret:= ( ::nArea )->( DbCloseArea() )
	 ::nArea := 0
return ret
