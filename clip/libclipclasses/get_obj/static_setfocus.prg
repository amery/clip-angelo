static function setFocus(self)
  local v
#ifdef DEBUG
	outlog(__FILE__,__LINE__,"__setfocus",self:name)
#endif
   if self:block==NIL .or. self:hasFocus
	return NIL
   endif
  //self:__oldReadVar:=set("__readvar",self:name)
  self:__oldReadVar:=set("__readvar",getReadVar(self))
  self:reset()
  self:hasFocus:=.t.

  self:original:=eval(self:block)
  self:__updateInfo()

  self:__analizePic()
  self:__winLen=min(self:__winlen,len(self:__format))

  if self:type=="N"
       self:__original:=str(self:_original,self:__len,self:__dec)
       self:__original:=strtran(self:__original,".","")
  endif

  self:__fillBuffer()
  if len(self:__posArr)>0
     self:pos:=self:__posArr[1]
  endif
  if self:type=="U" .or. (self:type=="C" .and. self:__original=="")
	self:typeOut:=.t.
  endif

  self:_display()

return NIL
