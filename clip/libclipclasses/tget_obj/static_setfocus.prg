static func setFocus()
  local v
  set("__varname",::name)
  ::reset()
  ::hasFocus:=.t.
  ::original:=eval(::block)
  ::type:=valtype(::original)
  ::edit:loadString(::original)
  ::display()
return NIL
