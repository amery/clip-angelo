*********************
static function me_enableBlockMenu()
local enable := iif(::__l_tobj,::tobj[::curwin]:strblock .or. ::tobj[::curwin]:rectblock,.f.)
	::oMenu:enableStatus(HASH_MarkBlockString, ::__l_tobj)
	::oMenu:enableStatus(HASH_MarkBlockRect  , ::__l_tobj)
	::oMenu:enableStatus(HASH_CancelBlock    , enable)
	::oMenu:enableStatus(HASH_CopyBlock      , enable)
	::oMenu:enableStatus(HASH_MoveBlock      , enable)
	::oMenu:enableStatus(HASH_DeleteBlock    , enable)
	::oMenu:enableStatus(HASH_CopyClipboard  , enable)
	::oMenu:enableStatus(HASH_PasteClipboard , !empty(::Clipboard))
	::oMenu:enableStatus(HASH_SaveBlock      , enable)
	::oMenu:enableStatus(HASH_LoadBlock      , ::__l_tobj)
	::oMenu:enableStatus(HASH_PrintBlock     , enable)
return