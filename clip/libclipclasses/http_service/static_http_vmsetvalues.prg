static function HTTP_VMsetValues(self)
	set macro_in_string off
	set translate path off
	set deleted on
	set device to printer
	set printer to membuf
	//outlog(__FILE__,__LINE__,self:dateFormat)
	set date format to (self:dateFormat)
return
