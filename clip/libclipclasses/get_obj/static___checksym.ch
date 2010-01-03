static function __checkSym(sym)
   local s:="",sf
#ifdef DEBUG
	outlog(__FILE__,__LINE__,"__checksym",::name)
#endif
   sf:=substr(::__format,::pos,1)
   if sf=="X" .or. sf==" "
      s=sym
   endif
   if sf=="A" .and. isalpha(sym)
      s=sym
   endif
   if sf=="N" .and. ( isalpha(sym) .or. isdigit(sym) )
      s=sym
   endif
   if sf=="9" .and. ( isdigit(sym) .or. sym $ "-+" )
      s=sym
   endif
   if sf=="#" .and. ( isdigit(sym) .or. sym $ "-+. " )
      s=sym
   endif
   if sf $ "YL" .and. ( upper(sym) $ (" TFYN"+LANG_YES_CHAR+LANG_NO_CHAR) )
      s=sym
   endif
   /*
   if sf=="Y" .and. ( upper(sym) $ (" YN"+LANG_YES_CHAR+LANG_NO_CHAR) )
      s=sym
   endif
   */
   if sf=="!" .or. "!" $ ::__flags
      s=upper(sym)
   endif
return s
