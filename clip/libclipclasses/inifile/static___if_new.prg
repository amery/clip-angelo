static function __IF_new(self,sFile)
	self:className	:="INIFILE"
	self:file	:= sFile
	self:data	:=map()
	self:__names	:=map()
	self:error	:=""
	_recover_inifile(self)
return self
