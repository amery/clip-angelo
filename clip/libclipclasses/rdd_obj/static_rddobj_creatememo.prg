static function rddobj_creatememo(Driver,Name)
  ::handle := ::handle
  Driver:=iif(Driver==nil,::memo_driver,Driver)
return rddcreatememo(Driver,Name)
