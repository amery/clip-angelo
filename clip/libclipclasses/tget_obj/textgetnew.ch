function TEXTGETNEW(row,col,bottom,right,block,varname,color,var,vblock,wblock)
  local obj
  obj:=map()
  obj:classname	:="TEXTGET"
  obj:block	:=block

  obj:buffer	:=NIL
  obj:original	:=NIL
  obj:cargo	:=NIL
  obj:changed	:=.f.
  obj:clear	:=.f.
  obj:col	:=col
  obj:row	:=row
  obj:nbottom	:=bottom
  obj:nright	:=right
  obj:colorSpec	:=iif(color==NIL,setcolor(),color)
  obj:decpos	:=0
  obj:exitState	:=GE_NOEXIT
  obj:hasFocus	:=.f.
  obj:minus	:=.f.
  obj:name	:=varname
  obj:picture	:=NIL
  obj:pos	:=1
  obj:postBlock	:=vblock
  obj:preBlock	:=wblock
  obj:reader	:=NIL
  obj:rejected	:=.f.
  obj:subscript	:=NIL
  obj:type	:="U"
  obj:caption	:=NIL
  obj:capRow	:=NIL
  obj:capCol	:=NIL
  obj:typeOut	:=.f.
  obj:control	:= NIL

  obj:__colors	:={}

  _recover_textget(obj)

  obj:__setcolor()

  obj:__firstKey:=.f.   // pressed first key/metod

  obj:edit:=textEditNew(row, col, bottom, Right, obj:colorSpec)
  obj:col	:=@ obj:edit:nLeft
  obj:row	:=@ obj:edit:nTop
  obj:pos	:=@ obj:edit:pos
  //obj:control	:=@ obj:edit

  obj:setFocus()
  obj:killFocus()

return obj
