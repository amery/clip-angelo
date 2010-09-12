static function tt_Add( nHours, nMinutes, nSeconds )
   local oTime := TTime():New( nHours, nMinutes, nSeconds )
   ::nTime += oTime:nTime
   ::nTime %= 86400
return ::GetTime()
