function _recover_sortedArray(obj)
	obj:len		:=@ __SA_len()

	obj:add		:=@ __SA_Add()
	obj:del		:=@ __SA_DelKey()
	obj:delete	:=@ __SA_DelKey()
	obj:delKey	:=@ __SA_delkey()
	obj:delItem	:=@ __SA_delItem()

	obj:seek	:=@ __SA_Seek()
	obj:hardSeek	:=@ __SA_hardSeek()
	obj:softSeek	:=@ __SA_SoftSeek()

	obj:getKey	:=@ __SA_getkey()
	obj:getData	:=@ __SA_getData()
	obj:getItem	:=@ __SA_getItem()
	obj:getFirst	:=@ __SA_getFirst()
	obj:getLast	:=@ __SA_getLast()
	obj:__bound	:=@ __SA_bound()
return obj
