static function rddobj_setindex(Driver,Name,Tag)
  __CHECKING_HANDLE
  Driver:=iif(Driver==nil,::index_driver,Driver)
return rddsetindex(::handle,Driver,Name,Tag)
