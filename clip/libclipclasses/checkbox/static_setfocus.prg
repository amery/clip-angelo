static func setFocus(self)
       self:hasFocus:=.t.
       self:__cursor:=setCursor(SC_NONE)
       self:display()
return self
