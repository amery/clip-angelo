static func setFocus(self)
       self:__cursor:=setcursor(SC_NONE)
       self:hasFocus:=.t.
       self:display()
return self
