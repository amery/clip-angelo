static function tdb_blank()
	local nrecno
( ::nArea )->( nRecNo := RecNo(), DBGoBottom(), DBSkip( 1 ), ;
               ::Load(), DBGoTo( nRecNo ) )
return nil
