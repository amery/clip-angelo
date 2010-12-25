static function rddobj_seek(Key,soft,last)
  __CHECKING_HANDLE
  soft:=iif(soft==nil,::soft_seek,soft)
  ::search_result:=rddseek(::handle,Key,soft,last)
return ::search_result
