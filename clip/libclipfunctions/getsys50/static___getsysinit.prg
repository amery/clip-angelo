static function __getSysInit()
   if status != NIL
      return
   endif
   status:=map()
   status:Format:=NIL
   status:Updated := .f.
   status:KillRead := .f.
   status:BumpTop:=NIL
   status:BumpBot:=NIL
   status:LastExit:=NIL
   status:LastPos:=NIL
   status:ActiveGet:=NIL
   status:ReadProcName:=NIL
   status:ReadProcLine:=NIL
return
