static function rddobj_eof()
  if ::handle==NIL; return .t.; endif
return rddeof(::handle)
