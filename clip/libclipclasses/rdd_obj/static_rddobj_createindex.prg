static function rddobj_createindex(Driver,Name,Tag,Expr,Unique)
  __CHECKING_HANDLE
  Driver:=iif(Driver==nil,::index_driver,Driver)
return rddcreateindex(::handle,Driver,Name,Tag,Expr,Unique)
