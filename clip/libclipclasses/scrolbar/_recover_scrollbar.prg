function _recover_scrollbar(obj)
	if obj:subtype=="VERTICAL"
  		obj:display	:= @sbv_display()
  		obj:update	:= @sbv_display()
  		obj:hitTest	:= @sbv_hitTest()
        else
  		obj:display	:= @sbh_display()
  		obj:update	:= @sbh_display()
  		obj:hitTest	:= @sbh_hitTest()
        endif
return
