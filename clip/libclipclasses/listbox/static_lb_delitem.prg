static func lb_delItem(self,pos)
     adel(self:__mText,pos)
     asize(self:__mText,len(self:__mtext)-1)
     adel(self:__mData,pos)
     asize(self:__mData,len(self:__mData)-1)
     self:itemCount--
     if self:itemCount==0
	self:typeOut:=.t.
     endif
     if self:__SB!=NIL
	self:__SB:total:=self:itemCount
     endif
     self:display()
return self
