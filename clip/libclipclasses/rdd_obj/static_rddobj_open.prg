static function rddobj_open(Driver,f_name,Shared,ReadOnly)
  if ::handle<>NIL; ::close(); endif
  Driver:=iif(Driver==nil,::table_driver,Driver)
  Shared:=iif(Shared==nil,::share_mode,Shared)
  ReadOnly:=iif(ReadOnly==nil,::read_only,ReadOnly)
  ::handle := rddusearea(Driver,f_name,Shared,ReadOnly)
return ::handle
