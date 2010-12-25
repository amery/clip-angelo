static function rddobj_create(Driver,FName,Stru)
  ::handle := ::handle
  Driver:=iif(Driver==nil,::table_driver,Driver)
return rddcreate(Driver,FName,Stru)
