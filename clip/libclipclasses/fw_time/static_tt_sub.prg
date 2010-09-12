static function tt_Sub( nHours, nMinutes, nSeconds )
   local oTime
   if ValType( nHours ) == "N"
      oTime = TTime():New( nHours, nMinutes, nSeconds )
   else
      oTime = nHours
   endif
   ::nTime -= oTime:nTime
   ::nTime %= 86400
return ::GetTime()
