static function tf_goto( nRecno )
return FSeek( ::hFile, nRecno, FS_SET )
