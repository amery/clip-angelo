function _recover_checkbox(obj)
   obj:display	:=@cb_display()
   obj:hitTest	:=@hitTest()
   obj:killFocus:=@killFocus()
   obj:select	:=@cb_select()
   obj:setFocus	:=@setFocus()
   obj:__setColor:=@__setcolor()
return obj
