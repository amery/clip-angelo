* function otvet
* parameters str - ������ ���������
*            y,x - ���������� ������ �������� ����
*            EscExit - .t. - ��� ������������ ��� ������� ESC ��� ������� lastkey()
*            color
* ****
// OTVET() ....
// EscExit==.f. .or. NIL - �� ����������� �� ������� ESC
//        ==.t. - �����
function otvet
local i,j,otv,oldcolor,scr
local row,col,mas[0],lenMas
local blD1,blD2,blN1,blN2
local _maxRowN:=maxRow()/2-2
local LenLastStr,color
local pName
parameters str,y,x,EscExit,colorP
//private __nm
local __nm
if funcname("PROV_BLANK")
   return .t.
endif
i:=1
iif(EscExit==NIL, EscExit:=.f., )
if funcname("OTVET")
   return .f.
endif
iif(y==NIL, y:=_maxRowN,)
blD1:=setkey(asc(LANG_YES_CHAR)       ,{|| in_key(__nm,.t.)})    // � || Y
blD2:=setkey(asc(lower(LANG_YES_CHAR)),{|| in_key(__nm,.t.)})    // � || y
blN1:=setkey(asc(LANG_NO_CHAR)        ,{|| in_key(__nm,.f.)})    // � || N
blN2:=setkey(asc(lower(LANG_NO_CHAR)) ,{|| in_key(__nm,.f.)})    // � || n
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
otv=.t.
lenMas:=len(mas)
 for i=1 to lenMas
   j=max(j,len(mas[i]))
 next
j=max(j,len(mas[len(mas)])+8)
color=IIF(colorP=NIL, "W/R,W+/R,,0,N/W", colorP)
oldcolor=setcolor(color)
x=iif(x==NIL,(80-j-7)/2,x)
y-=lenMas/2
LenLastStr=len(mas[lenMas])
do while .t.
   IIF(colorP=NIL, setcolor("W/R"), setcolor(color) )
   Dbox(y,x,y+1+lenMas,x+j+3)
   if EscExit
	  colorMess ( y, x+3, " Esc - none " )
   endif
   IIF(colorP=NIL,setcolor("W+/R,N/W,,0,W+/R"), )
   for i=1 to lenMas
	 dispOutAt( y+i,x+2, mas[i] )
   next
   i--
   @ y+i,x+2+LenLastStr+1 MPROMPT LANG_YES_STRING
   @ y+i,x+2+LenLastStr+5 MPROMPT LANG_NO_STRING
   MMENU TO __NM
   do case
	  case __nm=0 .and. EscExit
	  keyboard chr(27)
	  otv:=.f.
	  case __nm=1
	  otv=.t.
	  case __nm=2
	  otv=.f.
   endcase
   if lastkey()=13
	  exit
   endif
   if lastkey()=27 .and. EscExit
	  exit
   else
	  clearkey()
   endif
enddo
setkey(asc("Y")       ,blD1)    // � || Y
setkey(asc("y"),blD2)    // � || y
setkey(asc("N")        ,blN1)    // � || N
setkey(asc("n") ,blN2)    // � || n
if !EscExit
   clearkey()
endif
setcolor(oldcolor)
restore screen from scr
setpos(row,col)
return otv
