static func killFocus(self)
   if self:hasFocus
      setcursor(self:__cursor)
      self:hasFocus:=.f.
      self:display()
   endif
   if self:fblock!=NIL
       eval(self:fblock)
   endif
return self
