static function rddobj_rlock(nRecNo,lRelease)
  __CHECKING_HANDLE
return rddrlock(::handle, nRecNo, lRelease)
