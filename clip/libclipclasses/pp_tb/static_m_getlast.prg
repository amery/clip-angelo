static func m_getLast()
       local i:=::itemCount
       while i>=1
	   if ::__items[i]:enabled .and. !::__items[i]:disabled   .and. ::__items[i]:caption!=MENU_SEPARATOR
	      return i
	   endif
	   i--
       enddo
return 0
