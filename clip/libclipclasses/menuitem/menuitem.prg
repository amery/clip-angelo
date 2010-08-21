function menuitem(caption,data,shortcut,message,id)
   local obj

   if caption==NIL
      return NIL
   endif

   obj:=map()
   obj:className:= "MENUITEM"
   obj:caption	:= caption
   obj:cargo	:= NIL
   obj:checked	:= .f.
   obj:data	:= data
   obj:enabled	:= .t.
   obj:disabled	:= .f.
   obj:id	:= iif(valtype(id)=="N",id,0)
   obj:message	:= iif(message==NIL,"",message)
   obj:shortcut	:= iif(valtype(shortcut)=="N",shortcut,0)

#ifdef __UNIX__
   obj:style	:= "*>"
#else
   obj:style	:= "�"+chr(16)
#endif

   _recover_menuitem(obj)
return obj
