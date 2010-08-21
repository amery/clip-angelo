STATIC FUNCTION callUserFunc( browse, user_func, nKey )
   LOCAL Mode:=0,ret:=DE_CONT,s,i:=1,nTmp

   if empty(user_func)
	return ret
   endif

   DO CASE
	CASE nKey != 0
		mode := DE_EXCEPT
	CASE browse:hitEmpty
		mode := DE_EMPTY
	CASE browse:hitBottom
		mode := DE_HITBOTTOM
	CASE browse:hitTop
		mode := DE_HITTOP
	OTHERWISE
		mode := DE_IDLE
   ENDCASE

   nTmp = SELECT()
   if "(" $ user_func
	ret := &user_func
   else
	s := user_func+"("+alltrim(str(mode))+","+alltrim(str(browse:ColPos()))+")"
	ret := &s
	//ret:=clip( user_func, mode, browse:ColPos() )
   endif

   if ret==DE_CONT .and. mode==DE_EMPTY
	ret := DE_REFRESH
   endif
#ifdef DE_APPEND_SUPPORT
   if ret == DE_APPEND
	ret := DE_REFRESH
	m->__de_append_mode := .t.
	 goto bottom
	 browse:Down()
   else
	m->__de_append_mode := .f.
   endif
#else
   if ret == DE_APPEND
	ret := DE_REFRESH
   endif
#endif
   SELECT(nTmp)
   if used()
	browse:configure()
	browse:forceStable()
   endif
   if ret != 0
	ret := -1
   endif
return ret
