function progparse(s)
local a
local aCode:={}
local i,n,cTok,nTok
static x_keywords:={"if","else","endif","while","endw","for","next",;
		    "private","public","release","exit","loop",;
		    "proc","endp","call","write","return","push","pop",;
		    "param","block","elif","loadlib"}
memvar aCommStack
private aCommStack:={}
blprocs:={}
a:=lineparse(s,x_EOL)
for i:=1 to len(a)
  a[i]:=alltrim(a[i])
  aadd(aCode,nil)
  if substr(a[i],1,len(comm_char))==comm_char .or. empty(a[i])
    aCode[len(aCode)]:={x_comment,a[i]}
  else
    n:=at(' ',a[i])
    if n<>0
      cTok:=substr(a[i],1,n-1)
    else
      cTok:=a[i]
      n:=len(a[i])
    endif
    do case
    case ((nTok:=ascan(x_keywords,lower(cTok)))<>0) .and. ((nTok-1)<>x_loadlib)
      aCode[len(aCode)]:={nTok-1,commparse(aCode,nTok-1,substr(a[i],n+1))}
    case (nTok-1)==x_loadlib
      aCode[len(aCode)]:={x_comment,a[i]}
      loadprocs(substr(a[i],n+1),a)
    otherwise
      aCode[len(aCode)]:={x_eval,mkblk(a[i],i)}
    end case
  endif
next
chkstack(aCommStack)
release aCommStack
return aCode
