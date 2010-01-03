function DBox(T,L,B,R,ColorOsn,TypeBox,StrTop,StrBot,ColorDop)
	local old,len,chr,str, oldCur:=setCursor(0)
	local _maxRowN:=maxRow()
	if ColorOsn==NIL
		ColorOsn=setcolor()
	endif
	if ColorDop==NIL
		ColorDop=7
	endif
	old=setcolor(ColorOsn)
	if typeBox==NIL
		@ T,L TO B,R DOUBLE
		@ T+1,L+1 clear to B-1,R-1
	else
		@ T,L,B,R Box TypeBox
		@ T+1,L+1 clear to B-1,R-1
	endif
	if B < _maxrowN
#ifndef __CLIP__
		draw_rect(B+1,iif(L+2>0,L+2,0),B+1,iif(R<79,R+1,79),ColorDop)
#else
		dispattr(B+1,iif(L+2>0,L+2,0),B+1,iif(R<79,R+1,79),ColorDop)
#endif
	endif
	if R<79
#ifndef __CLIP__
		draw_rect(iif(T+1>0,T+1,0),R+1,iif(B<_maxRowN,B+1,B),R+iif(R==78,1,2),ColorDop)
#else
		dispattr(iif(T+1>0,T+1,0),R+1,iif(B<_maxRowN,B+1,B),R+iif(R==78,1,2),ColorDop)
#endif
	endif
	if StrTop!=NIL
		str=alltrim(padc(StrTop,R-L))
		len=len(StrTop)
		@ T,L+(R-L-len)/2 say StrTop
	endif
	if StrBot!=NIL
		str=alltrim(padc(StrBot,R-L))
		len=len(StrBot)
		@ B,L+(R-L-len)/2 say StrBot
	endif
	setcolor(old)
	setcursor(oldCur)
return
