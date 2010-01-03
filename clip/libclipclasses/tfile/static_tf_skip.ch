static function tf_skip( nBytes )
return FSeek( ::hFile, nBytes, FS_RELATIVE )
