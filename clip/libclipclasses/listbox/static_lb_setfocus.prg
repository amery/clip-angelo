static function lb_setFocus(self)
      self:__cursor:=setcursor(SC_NONE)
      self:hasFocus:=.t.
      if self:fblock != NIL
	 eval(self:fblock,.t.)
      endif
      self:display()
return self
