static func stabilize(self)
  local i,nskip,row,col,nstab,j,k
#ifdef DEBUG_CALL
	outlog(__FILE__,__LINE__,"stabilize")
#endif
   if empty(self:__columns)
	return .t.
   endif

  if !self:__firstStab
	 self:__setcolor()
	 self:__remakeColumns()
	 self:__whoVisible()
	 self:__firstStab:=.t.
  endif

  if self:__lRedrawTable
	self:__sayTable()
  endif

  self:rowPos=min(self:rowPos,self:rowCount)
  i:=ascan(self:__colVisible,self:colPos)
  if i>0
	self:__colPos := i
  else
	self:configure()
	return .f.
  endif

  if !self:stable
     nstab=0
     for j=1 to self:rowCount
       if self:__refreshLine[j]=0
	  nstab=1
	  self:__refreshLine[j]=1
	  nskip=j-self:rowPos
	  k:=eval(self:skipBlock,nskip)

	  if k == nskip
	     self:_outCurrent(j)
	  elseif nskip>0
	    row:=row()-1; col:=col()
	     for i=j to self:rowCount
		self:__dummyRow(i)
		self:__refreshLine[i]=1
	     next
	     //devpos(row,col)   // ?????

	  elseif nskip<0
	     self:rowPos:=min(self:rowPos,-k + 1)
	     self:refreshAll()
	  endif

	  eval(self:skipBlock,-k)

	  exit
       endif
     next
   if nstab=0
	self:stable=.t.
   endif
  endif
  self:_outCurrent()
return self:stable
