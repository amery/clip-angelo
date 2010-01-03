static function rddobj_setmemo(Driver,Name)
  __CHECKING_HANDLE
  Driver:=iif(Driver==nil,::memo_driver,Driver)
return rddsetmemo(::handle,Driver,Name)
