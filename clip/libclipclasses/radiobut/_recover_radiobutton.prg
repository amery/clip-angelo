function _recover_radiobutton(obj)
   obj:display	:=@rb_display()
   obj:hitTest	:=@hitTest()
   obj:isAccel	:=@rb_isAccel()
   obj:killFocus:=@killFocus()
   obj:select	:=@rb_select()
   obj:setFocus	:=@setFocus()
   obj:__setColor:=@__setcolor()
return obj
