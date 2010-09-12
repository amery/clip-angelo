static func aaaaaaaaaa(v,x,str,div,set)
static x1:=0,x2:=0
local _maxR,t,l,b,r,i,j
if set!=NIL .and. valtype(set)=="L"
    mset(_MSET_PROGRESS,set)
    return 0
endif
if mset(_MSET_PROGRESS)
	return 0
endif
_maxR:=maxRow()-2
T:=_maxR-2; L:=10; B:=_maxR+2; R:=70
do case
   case v<>NIL .and. v
       return 0
   case pcount()=0 .or. (pcount()=1 .and. v<>NIL .and. !v)
       x2++
       i:=(x2/x1)*100
       i:=abs(i)
       j:=iif(i>100,100,i)
       j:=int(j/2.5)
       dispOutAt( T+1,L+20, "��������� "+alltrim(str(i))+"%" )
       dispOutAt( T+3,L+10, replicate("�",j) )
   case pcount()==4 .and. div<>NIL
       x1+=div
       return 0
   case pcount()>1 .and. pcount()<4
       x1:=x
       x2:=0
       dbox(T,L,B,R)
       dispOutAt( T+2,L+10, padc(alltrim(str),40) )
       dispOutAt( T+3,L+10, replicate("�",40) )
endcase
return 0
