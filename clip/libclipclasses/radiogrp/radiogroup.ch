function radioGroup(top,left,bottom,right)
   local obj
   obj:=map()
   obj:classname:="RADIOGROUP"
   obj:bottom	:=bottom
   obj:buffer	:=0       // ������-�� � �������� ����� buffer,�
   obj:value	:=0        // ������-�� � �������� ����� buffer,�
			   // � getsys.prg - value ??????
   obj:capRow	:=top
   obj:capCol	:=left+2
   obj:caption	:=NIL
   obj:cargo	:=NIL
   obj:sayBox	:=.t.
   obj:coldbox	:=NIL //B_SINGLE+" "
   obj:colorSpec:=setcolor()
   obj:fblock	:=NIL
   obj:hasFocus	:=.f.
   obj:hotBox	:=NIL //B_DOUBLE+" "
   obj:itemCount:=0
   obj:left	:=left
   obj:message	:=""
   obj:right	:=right
   obj:top	:=top
   obj:typeOut	:=.t.


   obj:__items	:={}
   obj:__cursor	:= setcolor()
   obj:__colors:={}

   _recover_radiogroup(obj)

   obj:__setcolor()

return obj
