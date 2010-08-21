function alert(sMsg,mmes,colorp)
local str,i,j,k,m,n,x,y,oldcolor, scr, win:=wselect(), nwin
local row,col,mas[0],lenMas,lenMes
local color:="W+/R,W+/B,,0,N/W"
local pName, __ret:=0

if pcount()==0
   return 0
endif

while dispcount()>0
	  dispend()
enddo


dispbegin()
if win!=0
	nwin:=wopen(0, 0, maxrow(.t.), maxcol(.t.), .f.)
endif
row:=row(); col:=col()
oldcolor:=setcolor()
save screen to scr
if colorp!=NIL
   color=colorp
endif

if valtype(mmes)!="A"
   mmes={"OK"}
endif

str:=sMsg
do while !empty(str)
   k=at(";",str)
   j=k
   if k==0
	  j:=len(str)+1
   endif
   j=min(j,maxcol()-2)
   aadd(mas, substr(str,1,j-1) )
   str:=substr(str,j+iif(k!=0,1,0))
enddo
lenmes=len(mmes)
k=2;n=0
for i=1 to lenmes
   k+=len(mmes[i])+4
   n+=len(mmes[i])+2
next
lenMas=len(mas)
j=k
for i=1 to lenMas
   j=max(j,len(mas[i])+2)
next
setcolor(color)
x=int((maxcol()-j)/2) // -1
y=int((maxrow()-lenMas)/2) //-1

   DISPBOX( y , x , y+3+lenMas , x+j+1 , B_SINGLE+" "  )
   for i=1 to lenMas
	dispOutAt(y+i,x+1, padc(mas[i],j))
   next
   y+=i+1
   m=int((j-n)/(lenmes+1))
   x+=m+1 //+(j-m*lenmes-n)/2
   for j=1 to lenmes
	  @ y,x PROMPT " "+mmes[j]+" "
	  x+=len(mmes[j])+m+2
   next
dispend()
   MENU TO __ret
dispbegin()
setcolor(oldcolor)
restore screen from scr
setpos(row,col)
if nwin!=NIL
	wclose(nwin)
	wselect(win)
endif
dispend()
return __ret
