static func pp_open(self)
   if !self:isopen
      self:__scr:=savescreen(self:top,self:left,self:bottom,self:right)
      self:isopen:=.t.
      //self:current:=self:getFirst()
      self:display()
   endif
return self
