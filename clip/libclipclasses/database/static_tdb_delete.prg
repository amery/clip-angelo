static function tdb_delete()
   if ::lShared
      if ::Lock()
         ( ::nArea )->( DbDelete() )
         ::UnLock()
      else
         MsgAlert( [Can~t lock dataBase ! Please try again] )
      endif
   else
      ( ::nArea )->( DbDelete() )
   endif
return nil
