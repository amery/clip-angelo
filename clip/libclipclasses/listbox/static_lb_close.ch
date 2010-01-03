static func lb_close(self)
	if self:isopen
	   restscreen(self:top+iif(self:dropDown,1,0),self:left,self:bottom,self:right,self:__scr)
	   self:isopen:=.f.
	endif
return self
