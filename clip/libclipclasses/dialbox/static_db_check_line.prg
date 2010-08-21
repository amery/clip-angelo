static function db_check_line(line)
local i:=len(::inbuffer)
   if line<=0
      return
   endif

   for i=i to line-1
       aadd(::inbuffer,{})
   next
  ::lines:=len(::inbuffer)
return
