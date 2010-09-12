function _recover_get(obj)
  obj:badDate		:= @badDate()
  obj:assign 		:= @assign()
  obj:colorDisp 	:= @colorDisp()
  obj:display 		:= @display()
  obj:_display 		:= @_display()
  obj:killFocus 	:= @killFocus()
  obj:reload		:= @reload()
  obj:setFocus		:= @setFocus()
  obj:reset 		:= @reset()
  obj:undo 		:= @get_undo()
  obj:unTransform 	:= @unTransform()
  obj:updateBuffer 	:= @updateBuffer()
  //obj:updateBuffer 	:= @setFocus()
  obj:varGet 		:= @varGet()
  obj:varPut 		:= @varPut()
  obj:width 		:= @get_width()

  obj:setPos 		:= @cur_setPos()
  obj:end 		:= @cur_end()
  obj:home 		:= @cur_home()
  obj:left 		:= @cur_left()
  obj:right 		:= @cur_right()
  obj:gotoPos 		:= @cur_goto()
  obj:toDecPos 		:= @toDecPos()
  obj:wordLeft 		:= @wordLeft()
  obj:wordRight 	:= @wordRight()

  obj:backSpace    	:= @backSpace()
  obj:delLeft      	:= @backSpace()
  obj:delRight     	:= @delRight()
  obj:delete       	:= @delRight()
  obj:delEnd       	:= @delEnd()
  obj:delWordLeft  	:= @delWordLeft()
  obj:delWordRight 	:= @delWordRight()
  obj:hitTest 		:= @get_hitTest()
  obj:setcolor 		:= @get_setcolor()

  obj:Insert 		:= @Insert()
  obj:overStrike   	:= @overStrike()
  obj:__analizePic 	:= @__analizePic()   // analize PICTURE string
  obj:__fillBuffer 	:= @__fillBuffer()   // ����������� ������
  obj:__updateInfo 	:= @__updateInfo()   // ������ ����� ���������� � ���������
					// ��������� ����
  obj:__checkSym   	:= @__checkSym()
  obj:__toString   	:= @__toString()
  obj:__toString   	:= @__toString()
  obj:setKey   		:= @__setKey()

  __set_default_keys(obj)

return obj
