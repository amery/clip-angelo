static func pp_close(self)
  if self:isopen
     restscreen(self:top,self:left,self:bottom,self:right,self:__scr)
     self:isopen:=.f.
     //self:current:=0
     self:__begItem:=1
  endif
return self
