static function tf_AppByte( nByte )
	Fseek( ::hFile, 0, FS_END )
        ::PutByte( nByte )
        ::nLen++
return ::nLen
