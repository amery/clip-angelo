static func assign()
 local changed:=::changed
 if ::changed .and. ::type $ GETS_TYPES
    eval(::block,::edit:saveString())
    ::setFocus()
    ::changed:=changed
 endif
return NIL
