function readkeyb(x1,x2,str,var,str2,var2,colors,stringBox,TopMess, BotMess)
local getlist:={},scr:=savescreen(),old
local oldcol:=col(),oldrow:=row(),oldconf:=set(27,.t.)
local lenstr,lenvar,x
memvar _var
private _var
//old_fixed:=set(2,.f.)
old=iif( colors==NIL,setcolor(),setcolor(colors))
set cursor on
lenstr=len(str)
if valtype(str2)!="U"
  lenstr=max(len(str),len(str2))
endif
iif( topMess==NIL, topMess:='', )
iif( botMess==NIL, botMess:='', )
if len(topMess)>lenstr .or. len(botMess)>lenstr
   lenstr=max( len(topMess), len(botMess) )
endif
if valtype(x1)=="U" .or. valtype(x2)=="U"
   x1=int(maxrow()/2)-2
   x2=int((maxcol()-lenstr)/2)
endif
if valtype(str2)=="U"
  dbox(x1,x2,x1+3,x2+lenstr+1,,stringBox,TopMess,BotMess)
else
  dbox(x1,x2,x1+5,x2+lenstr+1,,stringBox,TopMess,BotMess)
endif
dispOutAt( x1+1,x2+1, padc(alltrim(str),lenstr) )
if valtype(str2)!="U"
  dispOutAt( x1+3,x2+1, padc(alltrim(str2),lenstr) )
endif
if funcname("PROV_BLANK") .or. funcname("PRINTBLANK")
  dispOutAt( x1+2,x2+1, padc(var,lenstr) )
  if valtype(var2)!="U"
	 dispOutAt( x1+4,x2+1, padc(var2,lenstr) )
  endif
else
  _var=var
  lenvar=slen("_var")
  x=int(max(1,(lenstr-lenvar)/2+1))
  if valtype(var)=="C" .and. "->"$var
	@ x1+2,x2+x get &var picture "@K@S"+alltrim(str(lenstr))
  else
	@ x1+2,x2+x get var picture "@K@S"+alltrim(str(lenstr))
  endif
  if valtype(var2)!="U"
	 _var=var2
	 lenvar=slen("_var")
*     lenvar=slen(var2)
	 x=int(max(1,(lenstr-lenvar)/2+1))
	 @ x1+4,x2+x get var2 picture "@K@S"+alltrim(str(lenstr))
   endif
  read
endif
setpos(oldrow,oldcol)
set(27,oldconf)
setcolor(old)
restore screen from scr
return var
