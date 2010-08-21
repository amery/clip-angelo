function getClrPair(color, num )
  local beg,end
  beg:=iif(num==0,0,at(",",color,num))
  end:=at(",",color,num+1)
  end:=iif(end==0,len(color), end)
return substr(color, beg+1, end-beg)
