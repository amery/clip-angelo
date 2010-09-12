static function tf_AppWord( nWord )
	Fseek( ::hFile, 0, FS_END )
	::PutWord( nWord )
	::nLen += 2
return ::nLen
