static function dev_out(colInfo,value)
	::outBuffer+=padr(toString(value,colInfo:len,colInfo:dec),colInfo:len)+" "
return
