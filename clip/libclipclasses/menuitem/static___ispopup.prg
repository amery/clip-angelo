static func __isPopUp
       local ret:=.f.
       if valtype(::data)=="O" .and. ::data:classname=="POPUPMENU"
          ret:=.t.
       endif
return ret
