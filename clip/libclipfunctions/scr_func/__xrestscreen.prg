//#command RESTORE SCREEN         => __XRestScreen()
function __XRestScreen()
   local scr
   if len(__Xscreen)==0
	  return
   endif
   scr:=atail(__Xscreen)
   restscreen(,,,,scr[1])
   setpos(scr[2], scr[3])
   asize(__Xscreen,len(__Xscreen)-1)
return
