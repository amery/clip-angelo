function _recover_iniFile(obj)
	obj:load	:=@ __IF_load()
	obj:save	:=@ __IF_save()
	obj:getValue	:=@ __IF_getValue()
	obj:setValue	:=@ __IF_setValue()
	obj:sections	:=@ __IF_sections()
	obj:keys	:=@ __IF_keys()
	obj:checkName 	:=@ __check_name()
return obj
