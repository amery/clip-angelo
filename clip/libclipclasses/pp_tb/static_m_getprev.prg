static func m_getPrev()
       local i:=::current-1
       while i>=1
	   if ::__items[i]:enabled .and. !::__items[i]:disabled  .and. ::__items[i]:caption!=MENU_SEPARATOR
	      return i
	   endif
	   i--
       enddo
return 0
