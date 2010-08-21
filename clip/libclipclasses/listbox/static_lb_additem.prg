static func lb_addItem(self,dtext,data,visible)
     aadd(self:__mText,dtext)
     aadd(self:__mData,data)
     self:itemCount++
     self:typeOut:=.f.
     if self:__SB!=NIL
	self:__SB:total:=self:itemCount
     endif
     if visible==NIL .or. visible
	self:display()
     endif
return self
