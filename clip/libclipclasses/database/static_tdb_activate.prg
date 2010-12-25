static function tdb_activate( )
   local OldArea:= Select()
   Select ( ::nArea )
   if ! Used()
      DbUseArea( .f., ::cDriver, ::cFile, ::cAlias, ::lShared, ::lReadOnly )
   endif
   Select ( OldArea )
return nil
