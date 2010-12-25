static func user_action(tobj, user_ret, mode, nkey)
local lExit:=.f.
    if user_ret = ME_TOGGLEWRAP
// not supported
    elseif user_ret = ME_TOGGLESCROLL
// not supported
    elseif user_ret = ME_WORDRIGHT
	tobj:wordRight()
    elseif user_ret = ME_BOTTOMRIGHT
	tobj:Bottom()
    elseif user_ret = ME_DEFAULT .and. nkey # nil
	if ! viewKeys(nkey,tobj, @lExit) .and. mode
	    editKeys(nkey,tobj, @lExit)
	endif
    elseif user_ret = ME_IGNORE
    elseif user_ret = ME_DATA .and. mode .and. nkey # nil .and. nkey >= 0
	editKeys(nkey,tobj, @lExit, .t.)
    elseif user_ret >= 1 .and. user_ret <= 31
	if ! viewKeys(user_ret,tobj, @lExit) .and. mode
	    editKeys(user_ret,tobj, @lExit)
	endif
    endif
return lExit
