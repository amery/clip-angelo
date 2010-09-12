static function tt_New(self, nHours, nMinutes, nSeconds )
   DEFAULT nHours TO 0, nMinutes TO 0, nSeconds TO 0
   self:nTime = iIf( nHours != 0, nHours * 3600, 0 ) + ;
             iIf( nMinutes != 0, ( nMinutes * 60 ), 0 ) + nSeconds
   self:nTime %= 86400
return self
