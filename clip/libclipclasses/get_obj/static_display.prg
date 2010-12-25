static function display(self)
   local s:=self:buffer,modify:=.f.,newValue
#ifdef DEBUG
	outlog(__FILE__,__LINE__,"display",self:name)
#endif
   if self:block==NIL
	return self
   endif
   self:__fillbuffer()
   if !(s==self:buffer) .and. s!=NIL
	self:buffer := s
	modify:=.t.
   endif
   newValue := eval(self:block)
   if !modify .and. !self:hasFocus .and. (self:buffer==NIL .or. valtype(self:_original) != valtype(newValue) .or. self:_original != newValue /* .or. !self:changed*/)
	self:__updateInfo()

	self:__analizePic()
	self:__winLen=min(self:__winlen,len(self:__format))

	if self:type=="N"
		self:__original:=str(self:_original,self:__len,self:__dec)
		self:__original:=strtran(self:__original,".","")
	endif

	self:__fillBuffer()
   endif
   self:_display()
   /*
   if self:winbuffer == nil
	devpos(self:row(),self:col+self:__winlen)
   endif
   */
return self
