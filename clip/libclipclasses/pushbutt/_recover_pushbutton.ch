function _recover_pushbutton(obj)
   obj:display	:=@pb_display()
   obj:hitTest	:=@hitTest()
   obj:killFocus:=@killFocus()
   obj:select	:=@pb_select()
   obj:setFocus	:=@setFocus()
   obj:__setColor:=@__setcolor()
return obj
