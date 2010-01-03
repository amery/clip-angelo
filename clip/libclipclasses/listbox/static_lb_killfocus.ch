static function lb_killFocus(self)
       setcursor(self:__cursor)
       self:hasFocus:=.f.
       if self:dropDown
	  self:close()
       endif
       if self:fblock != NIL
	  eval(self:fblock,.f.)
       endif
       self:display()
return self
