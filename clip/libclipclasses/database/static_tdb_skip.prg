static function tdb_skip(self, nRec )
   nRec := iif(nRec==NIL, 1, nRec)
   ( self:nArea )->( DbSkip( nRec ) )
   if self:lBuffer
      self:Load()
   endif

   if self:Eof()
      if self:bEoF != nil
         Eval( self:bEoF, Self )
      endif
   endif

   if self:BoF()
      if self:bBoF != nil
         Eval( self:bBoF, Self )
      endif
   endif

return nil
