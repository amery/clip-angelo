static func m_getItem(npos)
   local ret:=NIL
   if npos>0 .and. npos<=::itemCount
      ret := ::__items[npos]
   endif
return ret
