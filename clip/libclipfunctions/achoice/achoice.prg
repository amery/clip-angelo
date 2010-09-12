Func achoice( nT,nL,nB,nR,_massP,__lMass,FunName,nPos,nLine,color,BoxType,topMess,BotMess )
local retVal:=0,bb,column,lVal, i, oldCursor,oldcolor,_lmass:={}
local maxStrSS,__bbbb
local massp:=_massp
private __ac_data
for i=1 to len(massp)
	if len(massp[i])==0 .or. valtype(massp[i]) != "C"
		exit
	endif
next
i--
i:=max(i,1)
if i>len(massP) .and. len(massp)==0
	massp:=aclone(_massp)
	aadd(massP,"")
	__lMass:={.f.}
endif
__ac_data:=map()
__ac_data:lenMass:=i //len(massP)
__ac_data:nt:=nt
__ac_data:nl:=nl
__ac_data:nb:=min(nt+i-1+iif(pcount()>10,2,0),nb)
__ac_data:nr:=nr
__ac_data:massp:=massp
__ac_data:funname:=funname
__ac_data:nRowMass:=1
__ac_data:boxFlag:=.f.
__ac_data:flagUserF:=(!empty(FunName) .and. valtype(funname) $ "CB" )
__ac_data:x:=NIL
__ac_data:exit:=.t.
if empty(__ac_data:massP) .or. valtype(__ac_data:massP[1])!="C"
   return 0
endif
if empty(__ac_data:massP)
   return 0
endif
oldCursor:=setCursor(0)
oldcolor:=setcolor()
if valtype(color)=="C"
   setcolor(color)
else
  color:=setcolor()
end
if __lMass==NIL
   asize(_lMass,__ac_data:lenMass)
   afill(_lMass,.t.)
elseif valType(__lMass)=="L"
   asize(_lMass,__ac_data:lenMass)
   afill(_lMass,__lmass)
else
   _lmass=aclone(__Lmass)
endif
for i=len(_lMass) to __ac_data:lenMass
	aadd(_lMass,.t.)
next
for i=1 to __ac_data:lenMass
   if valtype(_lmass[i])=="C"
      __bbbb=_lmass[i]
      _lmass[i]=&__bbbb
   endif
   if valtype(_lmass[i])!="L"
      _lmass[i]=.t.
   endif
   if __ac_data:exit .AND. _lmass[i]
      __ac_data:exit:=.f.
   endif
next
__ac_data:_lmass:=_lmass
nPos:=max(iif(nPos==NIL, 1,nPos),1)
nPos:=min(__ac_data:lenMass,nPos)

if pcount()>10
   dBox(__ac_data:nT,__ac_data:nL,__ac_data:nB,__ac_data:nR,,BoxType,TopMess,BotMess)
   __ac_data:boxFlag:=.t.
else
    __ac_data:nT--; __ac_data:nL--; __ac_data:nB++; __ac_data:nR++
endif
bb := TBrowseNew(__ac_data:nT+1, __ac_data:nL+1, __ac_data:nB-1, __ac_data:nR-1)

if valtype(nLine)=="N"
	nLine++
endif
nLine:=max(iif(nLine==NIL, nPos,nLine),1)
if nLine > nPos
	nLine :=1
endif
nLine:=min( __ac_data:nB-__ac_data:nT-1,nLine)

bb:headSep :=""
bb:colSep := ""
bb:skipBlock := {|x| skipper(x)}
bb:goBottomBlock:={||__ac_data:nRowMass:=__ac_data:lenMass}
maxStrSS:=__ac_data:nR-__ac_data:nL+1
column := TBColumnNew( , {|| iif(memvar->__ac_data:nRowMass<=0,"",padr(memvar->__ac_data:massP[memvar->__ac_data:nRowMass],maxStrSS))})

column:cargo:=__ac_data:lenMass
column:colorBlock:={|x| iif( __ach_color_block() ,{1,2},{5,2})}
bb:addColumn(column)

eval(bb:skipBlock,npos-1)
bb:rowPos:=nLine
bb:forceStable()

RetVal:=runTBm( bb )
bb:deHilite()
setCursor(oldCursor)
setcolor(OLDcolor)
return RetVal
