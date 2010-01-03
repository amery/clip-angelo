static function tf_AppStr( cStr )
	Fseek( ::hFile, 0, FS_END )
        ::PutStr( cStr )
        ::nLen += len( cStr )
return ::nLen
