function listBox(top,left,bottom,right,ldrop)
   local obj
   ldrop:=iif(ldrop==NIL,.f.,ldrop)

   obj		:= map()
   //obj:name	:= ""
   //obj:subscript:= NIL
   obj:classname:="LISTBOX"
   obj:bitmap	:= NIL
   obj:bottom	:= bottom
   obj:buffer	:= 1
   obj:capCol	:= NIL
   obj:capRow	:= NIL
   obj:caption	:= ""
   obj:cargo	:= NIL
   obj:coldBox	:= NIL //B_SINGLE+" "
   obj:colorSpec:= setcolor()
   obj:dropDown	:= ldrop
   obj:fblock	:= NIL
   obj:hasFocus	:= .f.
   obj:hotBox	:= NIL //B_DOUBLE+" "
   obj:isOpen	:= !ldrop
   obj:useArrow := .t.
   obj:itemCount:= 0
   obj:left	:= left
   obj:message	:= ""
   obj:right	:= right
   obj:sblock	:= NIL
   obj:top	:= top
   obj:topItem	:= 1
   obj:typeOut	:= .t.
   obj:value	:= 0
   obj:vscroll	:= NIL

   _recover_listbox(obj)

   obj:__cursor	:= setcursor()
   obj:__line	:= obj:value
   obj:__mText	:= {}
   obj:__mData	:= {}
   obj:__SB	:= NIL
   obj:__colors	:= {}
   obj:__bufType:= 0  // О©╫О©╫О©╫ О©╫О©╫О©╫О©╫О©╫О©╫ О©╫ О©╫О©╫О©╫О©╫О©╫О©╫ - 0-О©╫О©╫О©╫О©╫О©╫,1-О©╫О©╫О©╫О©╫О©╫,2-О©╫О©╫О©╫О©╫О©╫О©╫
   obj:__scr	:= savescreen(top+iif(ldrop,1,0),left,bottom,right)
   obj:__setColor()

return obj
