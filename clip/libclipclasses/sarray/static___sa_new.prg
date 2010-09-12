static function __SA_New(self)
	self:className	:="SORTEDARRAY"
	self:items	:={}
	self:error	:=""
	self:found	:= .f.
	_recover_sortedarray(self)
return self
