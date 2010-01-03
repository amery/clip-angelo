static function get_undo(self)
	eval(self:block,self:original)
	self:reset()
	self:__updateInfo()
	self:__analizePic()
	self:__winLen=min(self:__winlen,len(self:__format))

	if self:type=="N"
		self:__original:=str(self:_original,self:__len,self:__dec)
		self:__original:=strtran(self:__original,".","")
	endif
	self:__fillbuffer()
	if len(self:__posArr)>0
		self:pos:=self:__posArr[1]
	endif
	self:_display()
return self
