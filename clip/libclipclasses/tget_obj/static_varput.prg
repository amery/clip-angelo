static func varPut(value)
if ::type $ GETS_TYPES
   return eval(::block,value)
   ::edit:loadString(value)
endif
return ::original
