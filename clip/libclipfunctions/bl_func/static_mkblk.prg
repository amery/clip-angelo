static function mkblk(s,i,ReadyBlock)
STATIC e
LOCAL r,errblk,c
e:=errorblock({|err| mkblkerr(err,i)})
begin sequence
  if ReadyBlock==nil
    c:=if(empty(s),'.t.',s)
    r:=&("{|| "+c+"}")
  else
    r:=&(s)
  endif
recover using errblk
  r:=nil
end sequence
errorblock(e)
return r
