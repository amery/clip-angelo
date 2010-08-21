function GETNEW(row,col,block,varname,pic,color,var,vblock,wblock)
  local obj,scr
#ifdef DEBUG
	outlog(__FILE__,__LINE__,"getnew",varname)
#endif
  obj		:= map()
  obj:classname	:= "GET"
  obj:block	:= block

  obj:buffer	:= NIL
  obj:original	:= NIL
  obj:cargo  	:= NIL
  obj:changed	:= .f.
  obj:clear	:= .f.
  obj:col	:= iif(col==NIL,col(),col)
  obj:row	:= iif(row==NIL,row(),row)
  obj:colorSpec	:= iif(empty(color),setcolor(),color)
  obj:decpos	:= 0
  obj:exitState	:= GE_NOEXIT
  obj:hasFocus	:= .f.
  obj:minus	:= .f.
  obj:name	:= varname
  obj:realName	:= varName
  obj:picture	:= pic
  obj:control	:= NIL
  obj:pos	:= 1
  obj:cursorPos	:= 1
  obj:postBlock	:= vblock
  obj:preBlock	:= wblock
  obj:reader	:= NIL
  obj:rejected	:= .f.
  obj:subscript	:= NIL
  obj:type	:= "U"
  obj:caption	:= ""
  obj:capRow	:= NIL
  obj:capCol	:= NIL
  obj:typeOut	:= .f.
  /* CLIP extension */
  obj:readOnly	:= .f.
  obj:expand	:= .f.

  //this metods and attribute not exist in CA-Clipper
  obj:__oldReadVar:=""

  obj:__firstKey:= .f. 		   // pressed first key/metod

  obj:_original	:= NIL
  obj:__original:= NIL             // original in string mode
  obj:__flags:=""                  // PICTURE functions
  obj:__format:=""                 // PICTURE format string
  obj:__dataSay:=""                // ! format symbols for "R"-function
  obj:__posArr:={}                 // array of position Gets symbols
  obj:__winLen:=0                  // S-function parameter
  obj:__maxLen:=0                  // S-function parameter really
  obj:__len:=NIL                   // only for numeric type
  obj:__dec:=NIL
  obj:__dateFormat:= NIL           // original date format
  obj:__fullScreen:=.t.

  obj:__first:=.t.
  obj:__colors:={}
  obj:__keys:=map()                // array for ::setkey()

  obj:winbuffer	:= nil

  _recover_get(obj)

  obj:setcolor()


  if block!=NIL
	dispbegin()
	scr:=savescreen()
	obj:setFocus()
	obj:killFocus()
	restscreen(,,,,scr)
	dispend()
  endif

return obj
