static func lb_insItem(self,pos,mtext,data)
     asize(self:__mText,len(self:__mtext)+1)
     ains(self:__mText,pos)
     self:__mText[pos] := mtext
     asize(self:__mData,len(self:__mData)-1)
     aadd(self:__mData,pos)
     self:__mData[pos] := data
     self:itemCount++
     self:typeOut:=.f.
     if self:__SB!=NIL
	self:__SB:total:=self:itemCount
     endif
     self:display()
return self
