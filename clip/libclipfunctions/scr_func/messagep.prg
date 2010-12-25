function messagep
local i,j,oldcolor,scr,oldCur,row,col
local mas[0],__colors:={},s
local _maxRowN:=maxRow()/2-2
local strmess:=LANG_ANYKEY_STRING
oldCur=setCursor(0)
parameters str,y,x,color
if color!=NIL
	   s:=color
	   while len(s)>0
	   i:=at(",",s)
	   i=iif(i==0,len(s)+1,i)
	   aadd(__colors,substr(s,1,i-1) )
	   s:=substr(s,i+1)
	   enddo
else
   aadd(__colors,"w/r")
   aadd(__colors,"w+/r")
endif
if funcname("PROV_BLANK")
   return .t.
endif
if y==NIL
	y=_maxRowN
endif
do while .t.
   j=at(";",str)
   if j!=0
	 aadd(mas,substr(str,1,j-1))
	 str:=substr(str,j+1)
   else
	aadd(mas,str)
	exit
   endif
enddo
 row=row()
 col=col()
 save screen to scr
 j:=len(strMess)
 for i=1 to len(mas)
   j=max(j,len(mas[i]))
 next
 oldcolor=setcolor(__colors[1])
 y-=(len(mas)+1)/2
 x=iif(x==NIL,(80-j-2)/2,x)
 @ y,x clear to y+1+len(mas), x+y+3
 Dbox(y,x,y+1+len(mas),x+j+3)
   for i=1 to len(mas)
	  dispOutAt( y+i,x+2, padr(mas[i],j), __colors[2] )
   next
   dispOutAt( y+1+len(mas),x+(j-len(strMess))/2+2, strMess, __colors[1] )
   inkey(0)
 setcolor(oldcolor)
 restore screen from scr
 setpos(row,col)
 setCursor(oldCur)
return .t.
