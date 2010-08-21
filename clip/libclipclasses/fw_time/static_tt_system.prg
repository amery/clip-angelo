static function tt_System(self)
   local cTime := Time()
   self:nTime = ( Val( SubStr( cTime, 1, 2 ) ) * 3600 ) + ;
             ( Val( SubStr( cTime, 4, 2 ) ) *   60 ) + ;
               Val( SubStr( cTime, 7, 2 ) )
return self
