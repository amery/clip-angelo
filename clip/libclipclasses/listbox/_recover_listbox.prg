function _recover_listbox(obj)
   obj:addItem	:= @lb_addItem()
   obj:close	:= @lb_close()
   obj:delItem	:= @lb_delItem()
   obj:display	:= @lb_display()
   obj:findText	:= @lb_findText()
   obj:getData	:= @lb_getData()
   obj:getItem	:= @lb_getItem()
   obj:getText	:= @lb_getText()
   obj:hitTest	:= @lb_hitTest()
   obj:insItem	:= @lb_insItem()
   obj:killFocus:= @lb_killFocus()
   obj:nextItem	:= @lb_nextItem()
   obj:nextPage	:= @lb_nextPage()
   obj:open	:= @lb_open()
   obj:prevItem	:= @lb_prevItem()
   obj:prevPage	:= @lb_prevPage()
   obj:scroll	:= @lb_scroll()
   obj:select	:= @lb_select()
   obj:setData	:= @lb_setData()
   obj:setFocus	:= @lb_setFocus()
   obj:setItem	:= @lb_setItem()
   obj:setText	:= @lb_setText()
   obj:__setBuffer:=@__setBuffer()
   obj:__setColor:= @__setcolor()
   //obj:reader  := {|a,b,c,d|GuiReader(a,b,c,d)}
return obj
