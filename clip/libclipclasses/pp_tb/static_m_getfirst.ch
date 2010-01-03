static func m_getFirst()
       local i
       for i=1 to ::itemCount
	   if ::__items[i]:enabled .and. !::__items[i]:disabled  .and. ::__items[i]:caption!=MENU_SEPARATOR
	      return i
	   endif
       next
return 0
