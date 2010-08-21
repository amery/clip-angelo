function ColorMess(y,x,str2,colorP,mass)
local prom,x1,y1,y2,len:=1,lenStr,SPpos,chr,i,EndChoice:=.t.
local colOsn,colDop
local lenchoice
//parameters y,x,str2,colorP,mass
lenStr:=len(str2)
if mass==NIL
   mass:={}
endif
i:=len(mass)
lenchoice:=x
if colorP==NIL
   colorP:=setcolor()
endif
colOsn:=colorP
*setcolor(colorP)
dispOutAt( y,x, str2, colOsn)
x1=substr(colorP,1,at("/",colorP)-1)    && "x1/y2,y1/ "
colorP=substr(colorP,at("/",colorP)+1)
y2=  substr(colorP,1,at(",",colorP)-1)
colorP=substr(colorP,at(",",colorP)+1)
y1=   substr(colorP,1,at("/",colorP)-1)
y1=iif(y1==x1,y2,y1)
y1=iif(y1==y2,iif(x1=="W+","GR+","W+"),y1)
*setcolor(y1+"/"+y2)
colDop:=y1+"/"+y2
do while len<=lenStr
   chr=substr(str2,len,1)
   if chr==" " .and. !EndChoice
	  mass[i][3]:=Len+x-2
	  EndChoice:=.t.
   endif
   if !(chr==" ") .and. EndChoice
	  aadd(mass,{0,0,0,0})
	  i++    // ������� ��-�� �������
	  mass[i][1]:=y
	  mass[i][2]:=Len+x-1
	  EndChoice:=.f.
   endif
   if (chr>="A" .and. chr<="Z") .or. (chr>="a" .and. chr<="z") .or. (chr>="0" .and. chr<="9")
	  dispOutAt( y,x+len-1, chr, colDop )
   endif
   len++
enddo
if i!=0 .and. mass[i][3]=0
   mass[i][3]=Len+x-2
endif
*setcolor(old)
return mass
