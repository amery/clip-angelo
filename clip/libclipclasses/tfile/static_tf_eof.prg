static function tf_eof( )
return FSeek( ::hFile, 0, FS_RELATIVE ) >= ::nLen
