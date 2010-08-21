static function tf_GetWord()
	local cWord := "  "
return If( FRead( ::hFile, @cWord, 2 ) == 2, Bin2I( cWord ), -1 )
