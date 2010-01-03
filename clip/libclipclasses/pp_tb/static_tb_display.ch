static func tb_display(self)
  local x,first:=.f.,i,xpos:=self:left,item
  dispbegin()
  if empty(self:__colors)
     self:__colors:=__splitColors(self:colorSpec)
     first:=.t.
  endif
  x:=self:right
  for i=self:current to 1 step -1
      item:=self:__items[i]
      x-=__capLength(item:caption)+2
  next
  i:=1
  for i=1 to self:itemCount
      if x>=self:left
	 exit
      endif
      item:=self:__items[i]
      x+=__capLength(item:caption)+2
  next
  self:__begItem:=i

  dispOutAt(self:row, self:left, space(self:right-self:left+1), self:colorSpec )
  for i=self:__begitem to self:itemCount
      item:=self:__items[i]
      __sayCaption(self:row,xpos," "+item:caption+" ",;
		   self:__colors[iif(self:current==i,2,1)],NIL, ;
		   self:__colors[iif(self:current==i,4,3)]  )
      if first .and. valtype(item)=="O" .and. valtype(item:data)=="O" .and.  item:data:classname=="POPUPMENU"
	 item:data:__resize(self:row+1,xpos)
      endif
      if xpos>self:right
		exit
      endif
      xpos+=__capLength(item:caption)+2
  next
  dispend()
return self
