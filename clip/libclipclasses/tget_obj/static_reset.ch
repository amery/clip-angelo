static func reset(obj)
  if obj:hasFocus
     obj:pos:=1
     obj:reader:=NIL
     obj:rejected:=.f.
     obj:subscript:=NIL
     obj:typeOut:=.f.
     obj:clear:=.f.
     obj:minus:=.f.
     obj:__firstKey:=.f.
  endif
return NIL
