static function tt_GetTime()
   local nTemp    := ::nTime
   local nHours   := Int( nTemp / 3600 )
   local nMinutes := nTemp - ( nHours * 3600 )
   local nSeconds := nMinutes % 60
return StrZero( nHours, 2 ) + ":" + StrZero( Int( nMinutes / 60 ), 2 ) + ":" + ;
       StrZero( nSeconds, 2 )
