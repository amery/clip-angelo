static func pp_display(self)
    local i,s,pal,pal2,item,first:=.f.,x,y,sSep
    if empty(self:__colors)
       self:__colors:=__splitColors(self:colorSpec)
       first:=.t.
    endif
    if !self:isopen .or. self:current==0
	return self
    endif
    dispbegin()
    if self:border == NIL
	sSep:=SEPARATOR_SINGLE
	@ self:top,self:left TO self:bottom,self:right ;
	   color self:__colors[6]
    else
	sSep:=substr(self:border,10)
	@ self:top,self:left,self:bottom,self:right ;
	   box substr(self:border,1,8) color self:__colors[6]
    endif
    self:__begItem:=  (self:top+self:current) - (maxrow()-2)+1
    if self:__begItem < 1
	self:__begItem:=1
    endif
    for i=self:__begItem to self:itemCount
	x:=self:top+i-self:__begItem+1
	if x > maxrow()-2
		exit
	endif
	item:=self:__items[i]
	if item:caption==MENU_SEPARATOR
	   s=substr(sSep,1,1)+;
	     replicate(substr(sSep,2,1),self:width+2)+;
	     substr(sSep,3,1)
	   dispOutAt(x, self:left, s, self:__colors[6])
	else
	   pal:=iif(self:current==i,2,1)
	   pal2:=iif(self:current==i,4,3)
	   pal:=iif(item:enabled .and. !item:disabled,pal,5)
	   pal2:=iif(item:enabled .and. !item:disabled,pal2,pal)
	   s:=iif(item:checked,substr(item:style,1,1)," ")
	   if first .and. valtype(item:data)=="O" .and.  item:data:classname=="POPUPMENU"
	      item:data:__resize(self:top+i,self:left+self:width+3)
	   endif
	   __sayCaption(x, self:left+1,;
			s+item:caption,;
			self:__colors[pal],self:width+2,self:__colors[pal2])
	   if valtype(item:data)=="O"
	      dispOutAt(x,self:left+self:width+2, substr(item:style,2,1), ;
			   self:__colors[pal] )
	   endif
	endif
    next
    dispend()
return self
